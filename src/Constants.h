#pragma once

// network
#define CLIENT_HOST "localhost"
#define CLIENT_PORT 8080
#define OSC_PORT    8004

// drawing
#define EYE_PERIPHERY_MULT  0.4f
#define EYE_PUPIL_SCL_MULT  0.35f
#define EYE_PUPIL_POS_MULT  0.15f
#define EYE_REST_TIMEDELTA  4000
//#define ALOWED_TRACK_PARTS  vector<string>{"nose", "leftElbow", "rightElbow"}
#define ALOWED_TRACK_PARTS  vector<string>{"nose"}

// strings
#define GUI_LABEL_CONNECT_TO_WS "Connect to WS"
#define GUI_LABEL_TOGGLE_DEBUG "Debug Mode"
#define SETTINGS_FILE_NAME "settings.xml"

// tracking
#define GRABBER_WIDTH       640
#define GRABBER_HEIGHT      480
