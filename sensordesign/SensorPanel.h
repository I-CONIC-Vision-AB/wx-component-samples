#pragma once

#include <wx/panel.h>
#include	<wx/textctrl.h>
#include	<wx/button.h>
#include	<wx/checkbox.h>
#include	<wx/radiobox.h>

class SensorPanel : public wxPanel
{
public:
	enum SensorPanelId
	{
		FIELD_OF_VIEW = 1001,
		FOCAL_LENGTH,
		FOCAL_LENGTH_PIXELS,
		TILT,
		NUM_CAMERAS,
		FOV_MODE,
		BPS_MODE,
		IMAGE_WIDTH_MM,
		IMAGE_WIDTH_PIXELS,
		IMAGE_HEIGHT_MM,
		IMAGE_HEIGHT_PIXELS,
		IMAGE_SIZE,
		NUM_MEGA_PIXELS,
		PIXEL_SIZE,
		IMAGE_RATIO,
		FLYING_HEIGHT,
		IMAGE_SCALE,
		GSD,
		OVERLAP,
		GROUND_WIDTH,
		GROUND_HEIGHT,
		LONGEST_PATH,
		LONGEST_PATH_FACTOR,
		SPEED,
		SPEED_MS,
		SPEED_KNTS,
		SHUTTER_SPEED,
		EXPOSURE_DISTANCE,
		EARTH_CURVATURE,
		AREA_PER_HOUR,
		NUM_IMAGES,
		DATA_PER_HOUR,
		EXPOSURE_INTERVAL,
		NET_GAIN_M,
		CLIPBOARD,
		NONE
	};

	static wxString gsTitles[33];

	SensorPanel() : wxPanel(NULL, wxID_ANY) {}

	SensorPanel(wxWindow* pParent, wxWindowID id = wxID_ANY, wxSize size = wxDefaultSize);

	void UpdateAll();

	double GetFieldOfViewBase();

	double GetFieldOfViewBasePixels();

	void OnCopyToClipboardVertical(wxCommandEvent& WXUNUSED(e));

protected:
	void OnFocalLength(wxCommandEvent &e);
	void OnFieldOfView(wxCommandEvent &WXUNUSED(e));
	void OnNumMegaPixels(wxCommandEvent &WXUNUSED(e));
	void OnFlyingHeight(wxCommandEvent &WXUNUSED(e));
	void OnOverlap(wxCommandEvent &WXUNUSED(e));
	void OnShutterSpeed(wxCommandEvent &WXUNUSED(e));
	void OnTilt(wxCommandEvent &WXUNUSED(e));
	void OnThreeCameras(wxCommandEvent &WXUNUSED(e));
	void OnFovMode(wxCommandEvent &WXUNUSED(e));
	void OnBpsMode(wxCommandEvent& WXUNUSED(e));
	void OnRatio(wxCommandEvent &WXUNUSED(e));
	void OnSpeed(wxCommandEvent& WXUNUSED(e));
	void OnImageWidth(wxCommandEvent& WXUNUSED(e));
	void OnImageHeight(wxCommandEvent& WXUNUSED(e));


	void OnCopyToClipboard(wxCommandEvent& WXUNUSED(e));

	void UpdateGroundSize();

	void UpdateSpeed();

	void UpdatePixelWidth();

	void UpdateNumImages();

	void UpdatePixelHeight();

	void UpdateDataPerHour();

	void UpdateImageSize();

	void UpdatePixelDimensions();

	void UpdateFocalLengthPixel();

	void UpdateGSD();

	void UpdateImageWidthMm();

	void UpdateImageHeightMm();

	void UpdateAfterOverlap();

	void UpdateFocalLengthMm();

	void UpdateMmPerPixel();

	void UpdateImageScale();

	void UpdateLongestPath();

	void UpdateExposureDistance();

	void UpdateEarthCurvature();

	void UpdateAreaPerHour();

	void UpdateExposureInterval();

	void UpdateNetGainMeter();

	wxTextCtrl *pFocalLengthCtrl, *pFocalLengthPixelsCtrl;
	wxTextCtrl *pFieldOfViewCtrl, *pTiltCtrl;
	wxTextCtrl *pWidthMmCtrl;
	wxTextCtrl *pNumPixelsCtrl;
	wxTextCtrl *pImageRatioCtrl;
	wxTextCtrl *pWidthPixelsCtrl;
	wxTextCtrl *pMmPerPixelCtrl;
	wxTextCtrl *pHeightCtrl;
	wxTextCtrl *pImageScaleCtrl;
	wxTextCtrl *pGsdCtrl;
	wxTextCtrl *pHeightMmCtrl;
	wxTextCtrl *pHeightPixelsCtrl;
	wxTextCtrl *pOverlapCtrl;
	wxTextCtrl *pGroundWidthCtrl, *pGroundHeightCtrl;
	wxTextCtrl *pSpeedCtrl, *pSpeedMsCtrl, *pSpeedKntsCtrl;
	wxTextCtrl *pEarthCurvatureCtrl;
	wxTextCtrl *pAreaPerHourCtrl;
	wxTextCtrl *pImageSizeCtrl;
	wxTextCtrl *pNumberOfImagesCtrl;
	wxTextCtrl *pExposureIntervalCtrl;
	wxTextCtrl *pDataPerHourCtrl;
	wxTextCtrl *pLongestPathCtrl, *pLongestPathFactorCtrl;
	wxTextCtrl *pShutterSpeedCtrl, *pExposureDistanceCtrl;
	wxCheckBox *cpNumCameras;
	wxRadioBox *cpFieldOfViewMode;
	wxRadioBox* cpBpsMode;
	wxTextCtrl *pNetGainKmCtrl;

	wxButton *cpClipboardBtn;

	double cFocalLength;
	int cFocalLengthPixels;
	double cFieldOfView, cTilt;
	double cImageWidthMm;
	double cImageHeightMm;
	int cNumMegaPixels;
	double cImageRatio;
	int cImageWidthPixels;
	int cImageHeightPixels;
	double cMmPerPixel;
	double cHeight;
	double cImageScale;
	double cGsd;
	double cOverlap;
	double cGroundWidth, cGroundHeight;
	double cLongestPath, cLongestPathFactor;
	double cSpeed, cSpeedMs, cSpeedKnts;
	int cShutterSpeed;
	double cExposureDistance;
	double cEarthCurvature;
	double cAreaPerHour;
	int cImageSize;
	int cNumImages;
	double cExposureInterval;
	bool cbHasCopiedTitles;
	int cDataPerHour;
	int cCameras;
	int cFovMode;
	int cBpsMode;
	double cNetGainMeter;

	wxWindowID cLastChanged;

	wxDECLARE_DYNAMIC_CLASS(SensorPanel);
	wxDECLARE_EVENT_TABLE();
};