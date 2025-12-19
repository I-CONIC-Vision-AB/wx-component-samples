#include	"SensorPanel.h"
#include	<wx/sizer.h>
#include	<wx/textctrl.h>
#include	<wx/log.h>
#include	<wx/statbox.h>
#include	<wx/clipbrd.h>
#include	<wx/checkbox.h>

wxIMPLEMENT_DYNAMIC_CLASS(SensorPanel, wxPanel);
wxBEGIN_EVENT_TABLE(SensorPanel, wxPanel)
EVT_TEXT(SensorPanel::FOCAL_LENGTH, SensorPanel::OnFocalLength)
EVT_TEXT(SensorPanel::FIELD_OF_VIEW, SensorPanel::OnFieldOfView)
EVT_TEXT(SensorPanel::NUM_MEGA_PIXELS, SensorPanel::OnNumMegaPixels)
EVT_TEXT(SensorPanel::FLYING_HEIGHT, SensorPanel::OnFlyingHeight)
EVT_TEXT(SensorPanel::OVERLAP, SensorPanel::OnOverlap)
EVT_TEXT(SensorPanel::SHUTTER_SPEED, SensorPanel::OnShutterSpeed)
EVT_TEXT(SensorPanel::TILT, SensorPanel::OnTilt)
EVT_TEXT(SensorPanel::IMAGE_RATIO, SensorPanel::OnRatio)
EVT_TEXT(SensorPanel::IMAGE_WIDTH_PIXELS, SensorPanel::OnImageWidth)
EVT_TEXT(SensorPanel::IMAGE_HEIGHT_PIXELS, SensorPanel::OnImageHeight)
EVT_TEXT(SensorPanel::SPEED, SensorPanel::OnSpeed)

EVT_CHECKBOX(SensorPanel::NUM_CAMERAS, SensorPanel::OnThreeCameras)
EVT_RADIOBOX(SensorPanel::FOV_MODE, SensorPanel::OnFovMode)
EVT_RADIOBOX(SensorPanel::BPS_MODE, SensorPanel::OnBpsMode)

EVT_BUTTON(SensorPanel::CLIPBOARD, SensorPanel::OnCopyToClipboard)
wxEND_EVENT_TABLE()

wxString SensorPanel::gsTitles[] = {
	"FIELD_OF_VIEW",
	"FOCAL_LENGTH",
	"FOCAL_LENGTH_PIXELS",
	"TILT",
	"NUM_CAMERAS",
	"FOV_MODE",
	"IMAGE_WIDTH_MM",
	"IMAGE_WIDTH_PIXELS",
	"IMAGE_HEIGHT_MM",
	"IMAGE_HEIGHT_PIXELS",
	"IMAGE_SIZE",
	"NUM_MEGA_PIXELS",
	"PIXEL_SIZE",
	"IMAGE_RATIO",
	"FLYING_HEIGHT",
	"IMAGE_SCALE",
	"GSD",
	"OVERLAP",
	"GROUND_WIDTH",
	"GROUND_HEIGHT",
	"LONGEST_PATH",
	"LONGEST_PATH_FACTOR",
	"SPEED",
	"SPEED_MS",
	"SPEED_KNTS",
	"SHUTTER_SPEED",
	"EXPOSURE_DISTANCE",
	"EARTH_CURVATURE",
	"AREA_PER_HOUR",
	"NUM_IMAGES",
	"DATA_PER_HOUR",
	"EXPOSURE_INTERVAL",
	"NET_GAIN_M"
};

SensorPanel::SensorPanel(wxWindow* pParent, wxWindowID id, wxSize size) :
	wxPanel(pParent, id, wxDefaultPosition, size),
	cFocalLength(0.0),
	cFieldOfView(60.0),
	cImageWidthMm(0.0),
	cImageHeightMm(0.0),
	cImageWidthPixels(4096.0),
	cImageHeightPixels(3000.0),
	cMmPerPixel(0.0),
	cNumMegaPixels(50),
	cImageRatio(36.0 / 24.0),
	cLastChanged(wxID_ANY),
	cHeight(10000),
	cImageScale(0.0),
	cGsd(0.0),
	cOverlap(80.0),
	cGroundWidth(0.0),
	cGroundHeight(0.0),
	cSpeed(850.0),
	cEarthCurvature(0.0),
	cbHasCopiedTitles(false),
	cShutterSpeed(100),
	cTilt(0.0),
	cCameras(1),
	cFovMode(1),
	cBpsMode(0)
{
	wxBoxSizer *pCameraSizer = new wxBoxSizer(wxVERTICAL);

	cpClipboardBtn = new wxButton(this, CLIPBOARD, _("&Copy to clipboard (or Ctrl+C)"));
	pCameraSizer->Add(cpClipboardBtn, 1, wxALL | wxEXPAND);
	cpClipboardBtn->SetToolTip(_("The copied text can be pasted into OpenOffice Calc or Excel documents"));

	wxBoxSizer *pHSizer = new wxBoxSizer(wxHORIZONTAL);
	pCameraSizer->Add(pHSizer, 1, wxALL | wxEXPAND);

	wxStaticBoxSizer *pFieldOfViewSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Field of view [degrees]"));
	pHSizer->Add(pFieldOfViewSizer, 1, wxALL | wxEXPAND);
	pFieldOfViewCtrl = new wxTextCtrl(this, FIELD_OF_VIEW, wxString::Format("%.1lf", cFieldOfView));
	pFieldOfViewSizer->Add(pFieldOfViewCtrl, 1, wxALL | wxEXPAND);
	pFieldOfViewCtrl->SetToolTip(_("Opening angle of camera sensor in degrees. Defined using the camera width. Field of view can also be defined using the camera diagonal or the height."));

	wxBoxSizer *pHSizer2 = new wxBoxSizer(wxHORIZONTAL);
	pHSizer->Add(pHSizer2, 1, wxALL | wxEXPAND);

	//wxStaticBoxSizer *pTiltSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Camera tilt [degrees]"));

	wxArrayString fovMode;
	fovMode.Add(_("Use diagonal"));
	fovMode.Add(_("Use width"));
	cpFieldOfViewMode = new wxRadioBox(this, FOV_MODE, _("Field of view mode"), wxDefaultPosition, wxDefaultSize, fovMode);
	pHSizer2->Add(cpFieldOfViewMode, 1, wxLEFT | wxEXPAND, 5);
	cpFieldOfViewMode->SetSelection(cFovMode);

	wxArrayString bpsMode;
	bpsMode.Add(_("8-bit"));
	bpsMode.Add(_("16-bit"));
	cpBpsMode = new wxRadioBox(this, BPS_MODE, _("Bits per sample"), wxDefaultPosition, wxDefaultSize, bpsMode);
	pHSizer2->Add(cpBpsMode, 1, wxLEFT | wxEXPAND, 5);
	cpBpsMode->SetSelection(cBpsMode);

	//wxStaticBoxSizer *pNumCamerasSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Camera configuration"));
	//pHSizer2->Add(pNumCamerasSizer, 1, wxLEFT | wxEXPAND, 5);
	//cpNumCameras = new wxCheckBox(this, NUM_CAMERAS, _("Three cameras across track"));
	//pNumCamerasSizer->Add(cpNumCameras, 1, wxLEFT | wxEXPAND, 5);
	//cpNumCameras->SetValue(false);

	//pHSizer2->Add(pTiltSizer, 1, wxALL | wxEXPAND);
	//pTiltCtrl = new wxTextCtrl(this, TILT, wxString::Format("%.1lf", cTilt));
	//pTiltSizer->Add(pTiltCtrl, 1, wxALL | wxEXPAND);
	//pTiltCtrl->SetToolTip(_("Tilt of camera."));

	pHSizer = new wxBoxSizer(wxHORIZONTAL);
	pCameraSizer->Add(pHSizer, 1, wxALL | wxEXPAND);

	wxStaticBoxSizer *pFocalLengthSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Focal length [mm]"));
	pHSizer->Add(pFocalLengthSizer, 1, wxALL | wxEXPAND);
	pFocalLengthCtrl = new wxTextCtrl(this, FOCAL_LENGTH, wxString::Format("%.1lf", cFocalLength));
	pFocalLengthSizer->Add(pFocalLengthCtrl, 1, wxALL | wxEXPAND);
	pFocalLengthCtrl->SetToolTip(_("35mm camera equivalent camera constant."));
	pFocalLengthCtrl->Enable(false);

	wxStaticBoxSizer *pFocalLengthPixelsSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Focal length [pixels]"));
	pHSizer->Add(pFocalLengthPixelsSizer, 1, wxALL | wxEXPAND);
	pFocalLengthPixelsCtrl = new wxTextCtrl(this, FOCAL_LENGTH_PIXELS, wxString::Format("%d", cFocalLengthPixels));
	pFocalLengthPixelsSizer->Add(pFocalLengthPixelsCtrl, 1, wxALL | wxEXPAND);
	pFocalLengthPixelsCtrl->Enable(false);
	pFocalLengthPixelsCtrl->SetToolTip(_("Camera constant in pixels."));
	pFocalLengthPixelsSizer->GetStaticBox()->SetToolTip(_("Camera constant in pixels."));

	pHSizer = new wxBoxSizer(wxHORIZONTAL);
	pCameraSizer->Add(pHSizer, 1, wxALL | wxEXPAND);

	wxStaticBoxSizer *pNumPixelsSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Pixels per camera [mega pixels]"));
	pHSizer->Add(pNumPixelsSizer, 1, wxALL | wxEXPAND);
	pNumPixelsCtrl = new wxTextCtrl(this, NUM_MEGA_PIXELS, wxString::Format("%d", cNumMegaPixels));
	pNumPixelsCtrl->Enable(false);
	pNumPixelsSizer->Add(pNumPixelsCtrl, 1, wxALL | wxEXPAND);
	pNumPixelsCtrl->SetToolTip(_("Number of pixels. Here we assume a 35x24 mm camera with 3:2 aspect ratio. Hence the width and height are derived from the number of pixels."));

	wxStaticBoxSizer *pImageSizeSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Image size 16-bit RGB [Mb]"));
	pHSizer->Add(pImageSizeSizer, 1, wxALL | wxEXPAND);
	pImageSizeCtrl = new wxTextCtrl(this, IMAGE_SIZE, wxString::Format("%d", cImageSize));
	pImageSizeCtrl->Enable(false);
	pImageSizeSizer->Add(pImageSizeCtrl, 1, wxALL | wxEXPAND);

	pHSizer = new wxBoxSizer(wxHORIZONTAL);
	pCameraSizer->Add(pHSizer, 1, wxALL | wxEXPAND);

	wxStaticBoxSizer *pWidthPixelsSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Image width [pixels]"));
	pHSizer->Add(pWidthPixelsSizer, 1, wxALL | wxEXPAND);
	pWidthPixelsCtrl = new wxTextCtrl(this, IMAGE_WIDTH_PIXELS, wxString::Format("%d", cImageWidthPixels));
	pWidthPixelsCtrl->Enable(true);
	pWidthPixelsSizer->Add(pWidthPixelsCtrl, 1, wxALL | wxEXPAND);

	wxStaticBoxSizer *pHeightPixelsSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Image height [pixels]"));
	pHSizer->Add(pHeightPixelsSizer, 1, wxALL | wxEXPAND);
	pHeightPixelsCtrl = new wxTextCtrl(this, IMAGE_HEIGHT_PIXELS, wxString::Format("%d", cImageHeightPixels));
	pHeightPixelsCtrl->Enable(true);
	pHeightPixelsSizer->Add(pHeightPixelsCtrl, 1, wxALL | wxEXPAND);

	pHSizer = new wxBoxSizer(wxHORIZONTAL);
	pCameraSizer->Add(pHSizer, 1, wxALL | wxEXPAND);

	wxStaticBoxSizer *pMmPerPixelSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Pixel size [mm/pixel]"));
	pHSizer->Add(pMmPerPixelSizer, 1, wxALL | wxEXPAND);
	pMmPerPixelCtrl = new wxTextCtrl(this, PIXEL_SIZE, wxString::Format("%.3lf", cMmPerPixel));
	pMmPerPixelCtrl->Enable(false);
	pMmPerPixelSizer->Add(pMmPerPixelCtrl, 1, wxALL | wxEXPAND);
	pMmPerPixelCtrl->SetToolTip(_("Physical dimensions of a pixel."));

	wxStaticBoxSizer *pImageRatioSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Width / height ratio"));
	pHSizer->Add(pImageRatioSizer, 1, wxALL | wxEXPAND);
	pImageRatioCtrl = new wxTextCtrl(this, IMAGE_RATIO, wxString::Format("%.3lf", cImageRatio));
	pImageRatioCtrl->Enable(false);
	pImageRatioSizer->Add(pImageRatioCtrl, 1, wxALL | wxEXPAND);

	pHSizer = new wxBoxSizer(wxHORIZONTAL);
	pCameraSizer->Add(pHSizer, 1, wxALL | wxEXPAND);

	wxStaticBoxSizer *pWidthMmSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Image width [mm]"));
	pHSizer->Add(pWidthMmSizer, 1, wxALL | wxEXPAND);
	pWidthMmCtrl = new wxTextCtrl(this, IMAGE_WIDTH_MM, wxString::Format("%.1lf", cImageWidthMm));
	pWidthMmCtrl->Enable(false);
	pWidthMmSizer->Add(pWidthMmCtrl, 1, wxALL | wxEXPAND);
	pWidthMmCtrl->SetToolTip(_("Physical width of image sensor."));

	wxStaticBoxSizer *pHeightMmSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Image height [mm]"));
	pHSizer->Add(pHeightMmSizer, 1, wxALL | wxEXPAND);
	pHeightMmCtrl = new wxTextCtrl(this, IMAGE_HEIGHT_MM, wxString::Format("%.1lf", cImageHeightMm));
	pHeightMmCtrl->Enable(false);
	pHeightMmSizer->Add(pHeightMmCtrl, 1, wxALL | wxEXPAND);
	pHeightMmCtrl->SetToolTip(_("Physical width of image sensor."));

	pHSizer = new wxBoxSizer(wxHORIZONTAL);
	pCameraSizer->Add(pHSizer, 1, wxALL | wxEXPAND);

	wxStaticBoxSizer *pGsdSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Ground sample distance (GSD) [m/pixel]"));
	pHSizer->Add(pGsdSizer, 1, wxALL | wxEXPAND);
	pGsdCtrl = new wxTextCtrl(this, GSD, wxString::Format("%.0lf", cGsd));
	pGsdCtrl->Enable(false);
	pGsdSizer->Add(pGsdCtrl, 1, wxALL | wxEXPAND);

	wxStaticBoxSizer *pImageScaleSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Image scale [1:x]"));
	pHSizer->Add(pImageScaleSizer, 1, wxALL | wxEXPAND);
	pImageScaleCtrl = new wxTextCtrl(this, IMAGE_SCALE, wxString::Format("%.0lf", cImageScale));
	pImageScaleCtrl->Enable(false);
	pImageScaleSizer->Add(pImageScaleCtrl, 1, wxALL | wxEXPAND);

	pHSizer = new wxBoxSizer(wxHORIZONTAL);
	pCameraSizer->Add(pHSizer, 1, wxALL | wxEXPAND);

	wxStaticBoxSizer *pGroundWidthSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Footprint width [m]"));
	pHSizer->Add(pGroundWidthSizer, 1, wxALL | wxEXPAND);
	pGroundWidthCtrl = new wxTextCtrl(this, GROUND_WIDTH, wxString::Format("%.0lf", cGroundWidth));
	pGroundWidthCtrl->Enable(false);
	pGroundWidthSizer->Add(pGroundWidthCtrl, 1, wxALL | wxEXPAND);

	wxStaticBoxSizer *pGroundHeightSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Footprint height [m]"));
	pHSizer->Add(pGroundHeightSizer, 1, wxALL | wxEXPAND);
	pGroundHeightCtrl = new wxTextCtrl(this, GROUND_HEIGHT, wxString::Format("%.0lf", cGroundHeight));
	pGroundHeightCtrl->Enable(false);
	pGroundHeightSizer->Add(pGroundHeightCtrl, 1, wxALL | wxEXPAND);

	pHSizer = new wxBoxSizer(wxHORIZONTAL);
	pCameraSizer->Add(pHSizer, 1, wxALL | wxEXPAND);

	wxStaticBoxSizer *pLongestPathSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Longest path through atmosphere [m]"));
	pHSizer->Add(pLongestPathSizer, 1, wxALL | wxEXPAND);
	pLongestPathCtrl = new wxTextCtrl(this, LONGEST_PATH, wxString::Format("%.0lf", cLongestPath));
	pLongestPathCtrl->Enable(false);
	pLongestPathSizer->Add(pLongestPathCtrl, 1, wxALL | wxEXPAND);

	wxStaticBoxSizer *pLongestPathFactorSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Longest path factor"));
	pHSizer->Add(pLongestPathFactorSizer, 1, wxALL | wxEXPAND);
	pLongestPathFactorCtrl = new wxTextCtrl(this, LONGEST_PATH_FACTOR, wxString::Format("%.2lf", cLongestPathFactor));
	pLongestPathFactorCtrl->Enable(false);
	pLongestPathFactorSizer->Add(pLongestPathFactorCtrl, 1, wxALL | wxEXPAND);

	pHSizer = new wxBoxSizer(wxHORIZONTAL);
	pCameraSizer->Add(pHSizer, 1, wxALL | wxEXPAND);

	wxStaticBoxSizer *pHeightSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Flying height [m]"));
	pHSizer->Add(pHeightSizer, 1, wxALL | wxEXPAND);
	pHeightCtrl = new wxTextCtrl(this, FLYING_HEIGHT, wxString::Format("%.0lf", cHeight));
	pHeightSizer->Add(pHeightCtrl, 1, wxALL | wxEXPAND);

	wxStaticBoxSizer *pAreaPerHourSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Area covered per hour [km*km]"));
	pHSizer->Add(pAreaPerHourSizer, 1, wxALL | wxEXPAND);
	pAreaPerHourCtrl = new wxTextCtrl(this, AREA_PER_HOUR, wxString::Format("%.0lf", cAreaPerHour));
	pAreaPerHourCtrl->Enable(false);
	pAreaPerHourSizer->Add(pAreaPerHourCtrl, 1, wxALL | wxEXPAND);

	wxStaticBoxSizer *pEarthCurvatureSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Max height correction for earth curvature [m]"));
	pCameraSizer->Add(pEarthCurvatureSizer, 1, wxALL | wxEXPAND);
	pEarthCurvatureCtrl = new wxTextCtrl(this, EARTH_CURVATURE, wxString::Format("%.3lf", cEarthCurvature));
	pEarthCurvatureSizer->Add(pEarthCurvatureCtrl, 1, wxALL | wxEXPAND);
	pEarthCurvatureCtrl->Enable(false);

	pHSizer = new wxBoxSizer(wxHORIZONTAL);
	pCameraSizer->Add(pHSizer, 1, wxALL | wxEXPAND);

	wxStaticBoxSizer *pSpeedSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Speed [km/h]"));
	pHSizer->Add(pSpeedSizer, 1, wxALL | wxEXPAND);
	pSpeedCtrl = new wxTextCtrl(this, SPEED, wxString::Format("%.0lf", cSpeed));
	pSpeedSizer->Add(pSpeedCtrl, 1, wxALL | wxEXPAND);
	pSpeedCtrl->Enable(true);

	wxStaticBoxSizer *pSpeedMsSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Speed [m/s]"));
	pHSizer->Add(pSpeedMsSizer, 1, wxALL | wxEXPAND);
	pSpeedMsCtrl = new wxTextCtrl(this, SPEED_MS, wxString::Format("%.0lf", cSpeedMs));
	pSpeedMsSizer->Add(pSpeedMsCtrl, 1, wxALL | wxEXPAND);
	pSpeedMsCtrl->Enable(false);

	wxStaticBoxSizer *pSpeedKntsSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Speed [knots]"));
	pHSizer->Add(pSpeedKntsSizer, 1, wxALL | wxEXPAND);
	pSpeedKntsCtrl = new wxTextCtrl(this, SPEED_KNTS, wxString::Format("%.0lf", cSpeedKnts));
	pSpeedKntsSizer->Add(pSpeedKntsCtrl, 1, wxALL | wxEXPAND);
	pSpeedKntsCtrl->Enable(false);

	pHSizer = new wxBoxSizer(wxHORIZONTAL);
	pCameraSizer->Add(pHSizer, 1, wxALL | wxEXPAND);

	wxStaticBoxSizer *pShutterSpeedSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Shutter speed [1:x s]"));
	pHSizer->Add(pShutterSpeedSizer, 1, wxALL | wxEXPAND);
	pShutterSpeedCtrl = new wxTextCtrl(this, SHUTTER_SPEED, wxString::Format("%d", cShutterSpeed));
	pShutterSpeedSizer->Add(pShutterSpeedCtrl, 1, wxALL | wxEXPAND);
	pShutterSpeedCtrl->Enable(true);
	pShutterSpeedCtrl->SetToolTip(_("A low shutter speed reduces motion blur, but also the radiometric dynamic range. Professional photogrammetric cameras have Forward Motion Compensation (FMC) to enable sharp images with long exposure times."));

	wxStaticBoxSizer *pExposureDistanceSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Distance during exposure [m]"));
	pHSizer->Add(pExposureDistanceSizer, 1, wxALL | wxEXPAND);
	pExposureDistanceCtrl = new wxTextCtrl(this, EXPOSURE_DISTANCE, wxString::Format("%.3lf", cExposureDistance));
	pExposureDistanceSizer->Add(pExposureDistanceCtrl, 1, wxALL | wxEXPAND);
	pExposureDistanceCtrl->Enable(false);

	pHSizer = new wxBoxSizer(wxHORIZONTAL);
	pCameraSizer->Add(pHSizer, 1, wxALL | wxEXPAND);

	wxStaticBoxSizer *pOverlapSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Overlap [%]"));
	pHSizer->Add(pOverlapSizer, 1, wxALL | wxEXPAND);
	pOverlapCtrl = new wxTextCtrl(this, OVERLAP, wxString::Format("%.0lf", cOverlap));
	pOverlapSizer->Add(pOverlapCtrl, 1, wxALL | wxEXPAND);

	wxStaticBoxSizer *pExposureIntervalSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Exposure interval [s]"));
	pHSizer->Add(pExposureIntervalSizer, 1, wxALL | wxEXPAND);
	pExposureIntervalCtrl = new wxTextCtrl(this, EXPOSURE_INTERVAL, wxString::Format("%.2lf", cExposureInterval));
	pExposureIntervalSizer->Add(pExposureIntervalCtrl, 1, wxALL | wxEXPAND);
	pExposureIntervalCtrl->Enable(false);

	pHSizer = new wxBoxSizer(wxHORIZONTAL);
	pCameraSizer->Add(pHSizer, 1, wxALL | wxEXPAND); 
	
	wxStaticBoxSizer *pNumImagesSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Number of images per hour"));
	pHSizer->Add(pNumImagesSizer, 1, wxALL | wxEXPAND);
	pNumberOfImagesCtrl = new wxTextCtrl(this, NUM_IMAGES, wxString::Format("%d", cNumImages));
	pNumImagesSizer->Add(pNumberOfImagesCtrl, 1, wxALL | wxEXPAND);
	pNumberOfImagesCtrl->Enable(false);

	wxStaticBoxSizer *pDataPerHourSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Image data [Gb/h]"));
	pHSizer->Add(pDataPerHourSizer, 1, wxALL | wxEXPAND);
	pDataPerHourCtrl = new wxTextCtrl(this, DATA_PER_HOUR, wxString::Format("%d", cDataPerHour));
	pDataPerHourSizer->Add(pDataPerHourCtrl, 1, wxALL | wxEXPAND);
	pDataPerHourCtrl->Enable(false);

	wxStaticBoxSizer *pNetGainKmSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Net gain per image [m]"));
	pHSizer->Add(pNetGainKmSizer, 1, wxALL | wxEXPAND);
	pNetGainKmCtrl = new wxTextCtrl(this, NET_GAIN_M, wxString::Format("%.0lf", cNetGainMeter));
	pNetGainKmSizer->Add(pNetGainKmCtrl, 1, wxALL | wxEXPAND);
	pNetGainKmCtrl->Enable(false);

	SetSizerAndFit(pCameraSizer);

	cLastChanged = IMAGE_WIDTH_PIXELS;
	UpdateAll();
}

void SensorPanel::UpdateSpeed()
{
	cSpeedMs = cSpeed * 1000.0 / 3600.0;
	pSpeedMsCtrl->ChangeValue(wxString::Format("%.0lf", cSpeedMs));

	cSpeedKnts = cSpeedMs * 1.9440;
	pSpeedKntsCtrl->ChangeValue(wxString::Format("%.0lf", cSpeedKnts));

}

void SensorPanel::UpdateMmPerPixel()
{
	cMmPerPixel = cImageWidthMm / cImageWidthPixels;
	pMmPerPixelCtrl->ChangeValue(wxString::Format("%.3lf", cMmPerPixel));
}

void SensorPanel::UpdateImageScale()
{
	cImageScale = cHeight / (cFocalLength*1.0e-3);
	pImageScaleCtrl->ChangeValue(wxString::Format("%.0lf", cImageScale));
}

void SensorPanel::UpdateLongestPath()
{
	double diag = std::sqrt(cGroundWidth*cGroundWidth + cGroundHeight * cGroundHeight);
	diag /= 2.0;
	cLongestPath = std::sqrt(diag*diag + cHeight * cHeight);
	pLongestPathCtrl->ChangeValue(wxString::Format("%.0lf", cLongestPath));

	cLongestPathFactor = cLongestPath / cHeight;
	pLongestPathFactorCtrl->ChangeValue(wxString::Format("%.2lf", cLongestPathFactor));

}

void SensorPanel::UpdateExposureDistance()
{
	cExposureDistance = cSpeedMs / cShutterSpeed;
	pExposureDistanceCtrl->ChangeValue(wxString::Format("%.3lf", cExposureDistance));
}

void SensorPanel::UpdateAreaPerHour()
{
	cAreaPerHour = cSpeed * cGroundWidth / 1000.0;
	pAreaPerHourCtrl->ChangeValue(wxString::Format("%.0lf", cAreaPerHour));
}

void SensorPanel::UpdateNetGainMeter()
{
	cNetGainMeter = ((1.0 - cOverlap / 100.0)*cGroundHeight);
	pNetGainKmCtrl->ChangeValue(wxString::Format("%.0lf", cNetGainMeter));
}

void SensorPanel::UpdateEarthCurvature()
{
	double diag = std::sqrt(cGroundWidth*cGroundWidth + cGroundHeight * cGroundHeight);
	diag /= 2.0;

	cEarthCurvature = diag*diag / (2.0 * 6370000.0);
	pEarthCurvatureCtrl->ChangeValue(wxString::Format("%.1lf", cEarthCurvature));

	// Atmospheric refraction correction
	double Z0 = cHeight / 1000.0; // Height in km
	double K = 2410.0 * Z0 / (Z0*Z0 - 6.0*Z0 + 250.0);
	K *= 1.0e-6;
	double dH = (diag*diag + cHeight * cHeight) / cHeight * K;
	dH *= 1.0;
}

double SensorPanel::GetFieldOfViewBase()
{
	double diag;
	switch (cFovMode)
	{
	case 0:
	default:
		diag = std::sqrt((cImageWidthMm / 2.0*cImageWidthMm / 2.0)*(1.0 + 1.0 / (cImageRatio*cImageRatio)));
		break;
	case 1:
		diag = cImageWidthMm / 2.0;
	}
	return diag;
}

double SensorPanel::GetFieldOfViewBasePixels()
{
	double diag;
	switch (cFovMode)
	{
	case 0:
	default:
		diag = std::sqrt((cImageWidthPixels / 2.0*cImageWidthPixels / 2.0)*(1.0 + 1.0 / (cImageRatio*cImageRatio)));
		break;
	case 1:
		diag = cImageWidthPixels / 2.0;
		
	}
	return diag;
}

void SensorPanel::UpdateGroundSize()
{
	double diag = 2.0 * cHeight * tan(cFieldOfView / 2.0 * M_PI / 180.0);
	if (cFovMode == 0)
	{
		cGroundHeight = diag*std::sqrt(1.0 / (1.0 + cImageRatio * cImageRatio));
		cGroundWidth = cImageRatio * cGroundHeight;
	}
	else
	{
		cGroundWidth = diag;
		cGroundHeight =  cGroundWidth / cImageRatio;
	}

	pGroundWidthCtrl->ChangeValue(wxString::Format("%.0lf", cGroundWidth));
	pGroundHeightCtrl->ChangeValue(wxString::Format("%.0lf", cGroundHeight));
}

void SensorPanel::UpdateImageWidthMm()
{
	if (cLastChanged == IMAGE_HEIGHT_PIXELS || cLastChanged == IMAGE_WIDTH_PIXELS) {
		cImageRatio = ((double)cImageWidthPixels) / cImageHeightPixels;
		pImageRatioCtrl->ChangeValue(wxString::Format("%.2lf", cImageRatio));
	}

	cImageWidthMm = cCameras * 24.0 *cImageRatio;
	pWidthMmCtrl->ChangeValue(wxString::Format("%.2lf", cImageWidthMm));
}

void SensorPanel::UpdateAll()
{
	UpdatePixelDimensions();
	UpdateSpeed();
	UpdateExposureDistance();
	UpdateImageWidthMm();
	UpdateImageHeightMm();
	UpdateImageSize();
	UpdateGroundSize();
	UpdateGSD();
	UpdateNetGainMeter();
	UpdateEarthCurvature();
	UpdateLongestPath();
	UpdateMmPerPixel();
	UpdateNumImages();
	UpdateDataPerHour();
	UpdateFocalLengthPixel();
	UpdateFocalLengthMm();
	UpdateImageScale();
	UpdateExposureInterval();
	UpdateAreaPerHour();
	cLastChanged = NONE;
	return;
}

void SensorPanel::UpdatePixelWidth()
{
	if (cLastChanged == IMAGE_WIDTH_PIXELS) return;
	//double w = cNumMegaPixels * cCameras * cImageRatio;
	//w = std::sqrt(w);
	//w *= 1024.0;
	//cImageWidthPixels = (int)(w + 0.5);

	pWidthPixelsCtrl->ChangeValue(wxString::Format("%d", cImageWidthPixels));
}

void SensorPanel::UpdatePixelHeight()
{
	if (cLastChanged == IMAGE_HEIGHT_PIXELS) return;
	//cImageHeightPixels = (int)(cNumMegaPixels * cCameras * 1024 * 1024 / cImageWidthPixels + 0.5);

	//int bps = cBpsMode == 0 ? 1 : 2; // bytes per sample
	//int numBands = 3;
	//cNumMegaPixels = cImageSize / (bps * numBands);
	pHeightPixelsCtrl->ChangeValue(wxString::Format("%d", cImageHeightPixels));
}

void SensorPanel::UpdatePixelDimensions()
{
	if (cLastChanged == IMAGE_WIDTH_PIXELS || cLastChanged == IMAGE_HEIGHT_PIXELS) {
		cNumMegaPixels = (int)(cImageWidthPixels * cImageHeightPixels / 1024 / 1024 + 0.5);
		pNumPixelsCtrl->ChangeValue(wxString::Format("%d", cNumMegaPixels));
		cImageRatio = ((double)cImageWidthPixels) / cImageHeightPixels;
		pImageRatioCtrl->ChangeValue(wxString::Format("%.2lf", cImageRatio));
		UpdateImageSize();
		return;
	}
	UpdatePixelWidth();
	UpdatePixelHeight();
}

void SensorPanel::UpdateFocalLengthPixel()
{
	double d = GetFieldOfViewBasePixels();
	cFocalLengthPixels = d / std::tan(cFieldOfView / 2.0 * M_PI / 180.0);
	pFocalLengthPixelsCtrl->ChangeValue(wxString::Format("%d", cFocalLengthPixels));
}

void SensorPanel::UpdateFocalLengthMm()
{
	cFocalLength = cImageWidthMm / cImageWidthPixels * cFocalLengthPixels;
	pFocalLengthCtrl->ChangeValue(wxString::Format("%.1lf", cFocalLength));
}

void SensorPanel::UpdateGSD()
{
	cGsd = cGroundWidth / cImageWidthPixels;
	pGsdCtrl->ChangeValue(wxString::Format("%.2lf", cGsd));
}

void SensorPanel::UpdateImageHeightMm()
{
	cImageHeightMm = 24.0;// cImageWidthMm / cImageRatio;
	pHeightMmCtrl->ChangeValue(wxString::Format("%.2lf", cImageHeightMm));
}

void SensorPanel::UpdateImageSize()
{
	int bps = cBpsMode == 0 ? 1 : 2; // bytes per sample
	int numBands = 3;
	cImageSize = bps * numBands * cNumMegaPixels;
	pImageSizeCtrl->ChangeValue(wxString::Format("%d", cImageSize));
}

void SensorPanel::UpdateDataPerHour()
{
	cDataPerHour = (int)(cNumImages * cCameras * cImageSize / 1024.0);
	pDataPerHourCtrl->ChangeValue(wxString::Format("%d", cDataPerHour));
}

void SensorPanel::UpdateNumImages()
{
	cNumImages = (int)(1000.0*cSpeed / cNetGainMeter + 0.5);
	pNumberOfImagesCtrl->ChangeValue(wxString::Format("%d", cNumImages));
}

void SensorPanel::UpdateExposureInterval()
{
	cExposureInterval = cNetGainMeter / cSpeedMs;
	pExposureIntervalCtrl->ChangeValue(wxString::Format("%.2lf", cExposureInterval));
}

void SensorPanel::UpdateAfterOverlap()
{
	if (cGroundHeight && cSpeedMs && cOverlap)
	{
		double kmGainPerImage = ((1.0 - cOverlap / 100.0)*(cGroundHeight / 1000.0));
		cExposureInterval = 1000.0 *kmGainPerImage / cSpeedMs;
		pExposureIntervalCtrl->ChangeValue(wxString::Format("%.2lf", cExposureInterval));
	}
}

void SensorPanel::OnFocalLength(wxCommandEvent &WXUNUSED(e))
{
	wxString s = pFocalLengthCtrl->GetValue();
	if (!s.ToDouble(&cFocalLength))
	{
		wxLogWarning(_("Focal length '%s' is not a number"), s);
		cFocalLength = 0.0;
		pFocalLengthCtrl->ChangeValue(wxString::Format("%.1lf", cFocalLength));
		return;
	}
	cLastChanged = FOCAL_LENGTH;
	UpdateAll();
}

void SensorPanel::OnFieldOfView(wxCommandEvent &WXUNUSED(e))
{
	wxString s = pFieldOfViewCtrl->GetValue();
	if (!s.ToDouble(&cFieldOfView))
	{
		wxLogWarning(_("Field of view '%s' is not a number"), s);
		cFieldOfView = 60.0;
		pFieldOfViewCtrl->ChangeValue(wxString::Format("%.1lf", cFieldOfView));
		return;
	}
	cLastChanged = FIELD_OF_VIEW;
	UpdateAll();
}

void SensorPanel::OnNumMegaPixels(wxCommandEvent &WXUNUSED(e))
{
	wxString s = pNumPixelsCtrl->GetValue();
	double numMegaPixels;
	if (!s.ToDouble(&numMegaPixels))
	{
		wxLogWarning(_("'%s' is not a number"), s);
		cNumMegaPixels = 0;
		pNumPixelsCtrl->ChangeValue(wxString("0"));
		return;
	}
	cNumMegaPixels = (int)(numMegaPixels + 0.5);
	cLastChanged = NUM_MEGA_PIXELS;
	UpdateAll();
}

void SensorPanel::OnSpeed(wxCommandEvent& WXUNUSED(e))
{
	wxString s = pSpeedCtrl->GetValue();
	if (!s.ToDouble(&cSpeed))
	{
		wxLogWarning(_("'%s' is not a number"), s);
		cSpeed = 100;
		pSpeedCtrl->ChangeValue(wxString::Format("%lf", cSpeed));
		return;
	}
	cLastChanged = SPEED;
	UpdateAll();
}

void SensorPanel::OnImageWidth(wxCommandEvent& WXUNUSED(e))
{
	wxString s = pWidthPixelsCtrl->GetValue();
	double ImageWidthPixels;
	if (!s.ToDouble(&ImageWidthPixels))
	{
		wxLogWarning(_("'%s' is not a number"), s);
		cImageWidthPixels = 0;
		pWidthPixelsCtrl->ChangeValue(wxString("0"));
		return;
	}
	cImageWidthPixels = (int)(ImageWidthPixels + 0.5);
	cLastChanged = IMAGE_WIDTH_PIXELS;
	UpdateAll();
}

void SensorPanel::OnImageHeight(wxCommandEvent& WXUNUSED(e))
{
	wxString s = pHeightPixelsCtrl->GetValue();
	double ImageHeightPixels;
	if (!s.ToDouble(&ImageHeightPixels))
	{
		wxLogWarning(_("'%s' is not a number"), s);
		cImageHeightPixels = 0;
		pHeightPixelsCtrl->ChangeValue(wxString("0"));
		return;
	}
	cImageHeightPixels = (int)(ImageHeightPixels + 0.5);
	cLastChanged = IMAGE_HEIGHT_PIXELS;
	UpdateAll();
}

void SensorPanel::OnRatio(wxCommandEvent &WXUNUSED(e))
{
	wxString s = pImageRatioCtrl->GetValue();
	if (!s.ToDouble(&cImageRatio))
	{
		wxLogWarning(_("'%s' is not a number"), s);
		cImageRatio = 1.5;
		pImageRatioCtrl->ChangeValue(wxString("1.5"));
		return;
	}
	cLastChanged = IMAGE_RATIO;
	UpdateAll();
}

void SensorPanel::OnFlyingHeight(wxCommandEvent &WXUNUSED(e))
{
	wxString s = pHeightCtrl->GetValue();
	if (!s.ToDouble(&cHeight))
	{
		wxLogWarning(_("'%s' is not a number"), s);
		cHeight = 0;
		pHeightCtrl->ChangeValue(wxString("0"));
		return;
	}
	cLastChanged = FLYING_HEIGHT;
	UpdateAll();
}

void SensorPanel::OnOverlap(wxCommandEvent &WXUNUSED(e))
{
	wxString s = pOverlapCtrl->GetValue();
	if (!s.ToDouble(&cOverlap))
	{
		wxLogWarning(_("'%s' is not a number"), s);
		cOverlap = 0;
		pOverlapCtrl->ChangeValue(wxString("0"));
		return;
	}
	cLastChanged = OVERLAP;
	UpdateAll();
}

void SensorPanel::OnCopyToClipboard(wxCommandEvent& WXUNUSED(e))
{
	if (wxTheClipboard->Open())
	{
		wxString s;
		for (int i = 0; i < CLIPBOARD - FOCAL_LENGTH; i++)
		{
			if (i>0)
				s += ',' + gsTitles[i];
			else
				s += gsTitles[i];
		}
		s += '\n';

		s += wxString::Format("%.2lf,", cFocalLength);
		s += wxString::Format("%d,", cFocalLengthPixels);
		s += wxString::Format("%.2lf,", cFieldOfView);
		s += wxString::Format("%.2lf,", cTilt);
		s += wxString::Format("%d,", cCameras);
		s += wxString::Format("%d,", cFovMode);
		s += wxString::Format("%.2lf,", cImageWidthMm);
		s += wxString::Format("%d,", cImageWidthPixels);
		s += wxString::Format("%.2lf,", cImageHeightMm);
		s += wxString::Format("%d,", cImageHeightPixels);
		s += wxString::Format("%d,", cImageSize);
		s += wxString::Format("%d,", cNumMegaPixels);
		s += wxString::Format("%.3lf,", cMmPerPixel);
		s += wxString::Format("%.2lf,", cImageRatio);
		s += wxString::Format("%.2lf,", cHeight);
		s += wxString::Format("%.2lf,", cImageScale);
		s += wxString::Format("%.2lf,", cGsd);
		s += wxString::Format("%.2lf,", cOverlap);
		s += wxString::Format("%.2lf,", cGroundWidth);
		s += wxString::Format("%.2lf,", cGroundHeight);
		s += wxString::Format("%.0lf,", cLongestPath);
		s += wxString::Format("%.3lf,", cLongestPathFactor);
		s += wxString::Format("%.2lf,", cSpeed);
		s += wxString::Format("%.2lf,", cSpeedMs);
		s += wxString::Format("%.2lf,", cSpeedKnts);
		s += wxString::Format("%d,", cShutterSpeed);
		s += wxString::Format("%.3lf,", cExposureDistance);
		s += wxString::Format("%.2lf,", cEarthCurvature);
		s += wxString::Format("%.2lf,", cAreaPerHour);
		s += wxString::Format("%d,", cNumImages);
		s += wxString::Format("%d,", cDataPerHour);
		s += wxString::Format("%.2lf,", cExposureInterval);
		s += wxString::Format("%.0lf", cNetGainMeter);
		s += '\n';

		wxTheClipboard->SetData(new wxTextDataObject(s));
		wxTheClipboard->Close();

		wxLogStatus(_("Data was copied to clipboard"));
	}
}

void SensorPanel::OnCopyToClipboardVertical(wxCommandEvent& WXUNUSED(e))
{
	if (wxTheClipboard->Open())
	{
		wxString s;
		int i = 0;
		s += wxString::Format("%s\t%.2lf\n", gsTitles[i++], cFieldOfView);
		s += wxString::Format("%s\t%.2lf\n", gsTitles[i++], cFocalLength);
		s += wxString::Format("%s\t%d\n", gsTitles[i++], cFocalLengthPixels);
		s += wxString::Format("%s\t%.2lf\n", gsTitles[i++], cTilt);
		s += wxString::Format("%s\t%d\n", gsTitles[i++], cCameras);
		s += wxString::Format("%s\t%d\n", gsTitles[i++], cFovMode);
		s += wxString::Format("%s\t%.2lf\n", gsTitles[i++], cImageWidthMm);
		s += wxString::Format("%s\t%d\n", gsTitles[i++], cImageWidthPixels);
		s += wxString::Format("%s\t%.2lf\n", gsTitles[i++], cImageHeightMm);
		s += wxString::Format("%s\t%d\n", gsTitles[i++], cImageHeightPixels);
		s += wxString::Format("%s\t%d\n", gsTitles[i++], cImageSize);
		s += wxString::Format("%s\t%d\n", gsTitles[i++], cNumMegaPixels);
		s += wxString::Format("%s\t%.3lf\n", gsTitles[i++], cMmPerPixel);
		s += wxString::Format("%s\t%.2lf\n", gsTitles[i++], cImageRatio);
		s += wxString::Format("%s\t%.2lf\n", gsTitles[i++], cHeight);
		s += wxString::Format("%s\t%.2lf\n", gsTitles[i++], cImageScale);
		s += wxString::Format("%s\t%.2lf\n", gsTitles[i++], cGsd);
		s += wxString::Format("%s\t%.2lf\n", gsTitles[i++], cOverlap);
		s += wxString::Format("%s\t%.2lf\n", gsTitles[i++], cGroundWidth);
		s += wxString::Format("%s\t%.2lf\n", gsTitles[i++], cGroundHeight);
		s += wxString::Format("%s\t%.0lf\n", gsTitles[i++], cLongestPath);
		s += wxString::Format("%s\t%.3lf\n", gsTitles[i++], cLongestPathFactor);
		s += wxString::Format("%s\t%.2lf\n", gsTitles[i++], cSpeed);
		s += wxString::Format("%s\t%.2lf\n", gsTitles[i++], cSpeedMs);
		s += wxString::Format("%s\t%.2lf\n", gsTitles[i++], cSpeedKnts);
		s += wxString::Format("%s\t%d\n", gsTitles[i++], cShutterSpeed);
		s += wxString::Format("%s\t%.2lf\n", gsTitles[i++], cExposureDistance);
		s += wxString::Format("%s\t%.2lf\n", gsTitles[i++], cEarthCurvature);
		s += wxString::Format("%s\t%.2lf\n", gsTitles[i++], cAreaPerHour);
		s += wxString::Format("%s\t%d\n", gsTitles[i++], cNumImages);
		s += wxString::Format("%s\t%d\n", gsTitles[i++], cDataPerHour);
		s += wxString::Format("%s\t%.2lf\n", gsTitles[i++], cExposureInterval);
		s += wxString::Format("%s\t%.2lf\n", gsTitles[i++], cNetGainMeter);

		wxTheClipboard->SetData(new wxTextDataObject(s));
		wxTheClipboard->Close();
		wxLogStatus(_("Data was copied to clipboard"));
	}
}

void SensorPanel::OnShutterSpeed(wxCommandEvent &WXUNUSED(e))
{
	wxString s = pShutterSpeedCtrl->GetValue();
	double d;
	if (!s.ToDouble(&d))
	{
		wxLogWarning(_("'%s' is not a number"), s);
		cShutterSpeed = 100;
		pShutterSpeedCtrl->ChangeValue(wxString::Format("%d", cShutterSpeed));
		return;
	}
	cShutterSpeed = (int)(d + 0.5);
	pShutterSpeedCtrl->ChangeValue(wxString::Format("%d", cShutterSpeed));

	cLastChanged = SHUTTER_SPEED;
	UpdateAll();
}

void SensorPanel::OnTilt(wxCommandEvent &WXUNUSED(e))
{
	wxString s = pTiltCtrl->GetValue();
	if (!s.ToDouble(&cTilt))
	{
		wxLogWarning(_("'%s' is not a number"), s);
		cTilt = 0.0;
		pTiltCtrl->ChangeValue(wxString::Format("%d", cTilt));
		return;
	}

	cLastChanged = TILT;
	UpdateAll();
}

void SensorPanel::OnThreeCameras(wxCommandEvent &WXUNUSED(e))
{
	cpNumCameras->GetValue() ? cCameras = 3 : cCameras = 1;

	cLastChanged = NUM_CAMERAS;
	UpdateAll();
}

void SensorPanel::OnFovMode(wxCommandEvent &WXUNUSED(e))
{
	cFovMode = cpFieldOfViewMode->GetSelection();
	cLastChanged = FOV_MODE;
	UpdateAll();
}

void SensorPanel::OnBpsMode(wxCommandEvent& WXUNUSED(e))
{
	cBpsMode = cpBpsMode->GetSelection();
	cLastChanged = BPS_MODE;
	UpdateAll();
}
