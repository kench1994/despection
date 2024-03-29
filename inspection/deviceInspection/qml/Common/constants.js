/*
* This file is part of syberos-settings
*
* Copyright (C) 2014 Beijing Yuan Xin Technology Co.,Ltd. All rights reserved.
*
* Authors:
*       Lele Huan <huanlele@syberos.com>
*       Zhenbin Cai <caizhenbin@syberos.com>
*
* This software, including documentation, is protected by copyright controlled
* by Beijing Yuan Xin Technology Co.,Ltd. All rights are reserved.
*/

var rate = 1;//applets.getSizeRatio();
//console.log("rate = " + rate)
/*pasword*/
var PASS_WORD_SIZE = 20 * rate
var POP_MENU_ITEM_HEIGHT = 80 * rate
var POP_MENU_FONT_SIZE = 30 * rate
var SYSTEM_FONT_SMALL = 20 * rate
var SYSTEM_FONT_DEFAULT = 40 * rate
var SYSTEM_FONT_MIDDLE = 50 * rate
var SYSTEM_FONT_BIG = 70 * rate
/* 根据张玉华效果图 */

var STATUSBAR_HEIGHT = 48 * rate
var STATUSBAR_MARGIN_BOTTOM = 12 * rate

var DEFAULT_MARGIN_LEFT = 40 * rate
var DEFAULT_MARGIN_RIGHT = 60 * rate
var DEFAULT_FONT_FAMILY = "方正兰亭准黑_GBK"//"方正兰亭准黑_GBK"; //"Liberation Sans" // "Droid Sans"

var SUBPAGE_MARGIN_LEFT = 50 * rate
var SUBPAGE_TITLE_FONT_SIZE = 36 * rate

/*dialog  style*/
var DIALOG_TITLE_SIZE = 40 * rate
var DIALOG_TITLE_FAMILY = DEFAULT_FONT_FAMILY
var DIALOG_TITLE_COLOR = "#282828"
var DIALOG_INPUT_SIZE = 40 * rate
var DIALOG_INPUT_FAMILY = DEFAULT_FONT_FAMILY
var DIALOG_INPUT_COLOR = "#FFFFFF"
var DIALOG_BUTTON_TITLE_SIZE = 40 *rate
var DIALOG_BUTTON_TITLE_FAMILY = DEFAULT_FONT_FAMILY
var DIALOG_BUTTON_TITLE_COLOR = "#282828"
var DIALOG_BUTTON_HEIGHT = 100 * rate
var DIALOG_BUTTON_COLOR = "#C5C1AA"
var DIALOG_BUTTON_PRESSED_COLOR = "#FFFAF0"
var DIALOG_TITLE_HEIGHT = 100 * rate
var DIALOG_INPUT_CONTAIN_HEIGHT = 100 * rate
var DIALOG_INPUT_HEIGHT = 60 * rate
var DIALOG_SEPORATOR_COLOR = "#D1D1D1"
var PADDING_XLARGE = 16 * rate
var TOUCH_EXPANSION_MARGIN = -12 * rate
var COLOR_SECONDARY_FOREGROUND = "#8c8c8c"
var COLOR_INVERTED_FOREGROUND = "#8c8c8c"

var GROUP_SEPARATOR_HIGHT = 20 * rate

var SECTION_HEIGHT = 80 * rate
var SECTION_FONT_SIZE = 32 * rate

//列表背景颜色
//var LIST_ITEM_BGCOLOR = "#34be44"
var LIST_ITEM_BGCOLOR = "#d9d9d9"
var WIFI_CONNECTED_COLOR = "#1ca72b"
var LIST_ITEM_HEIGHT = 80 * rate
var LIST_ITEM_ROW_SPACEING = 20 * rate
var LIST_TITLE_MARGIN_LEFT = 20 * rate
var LIST_TITLE_FAMILY = DEFAULT_FONT_FAMILY
var LIST_TITLE_SIZE = 22 * rate
//var LIST_TITLE_COLOR = "#000000"
var LIST_TITLE_COLOR = "#333333"
var LIST_PRESSED_COLOR = "#ffffff"
var LIST_TITLE_DISABLE_COLOR = "#c2cccc"
var LIST_TITLE_COLOR_PRESSED = "#797979"
var LIST_TITLE_COLOR_INVERTED = "#ffffff"
var LIST_TITLE_COLOR_PRESSED_INVERTED = "#797979"

var LIST_SUBTITLE_FAMILY = DEFAULT_FONT_FAMILY
var LIST_SUBTILE_SIZE = 18 * rate
var LIST_SUBTITLE_COLOR = "#999999"
var LIST_SUBTITLE_COLOR_PRESSED = "#797979"
var LIST_SUBTITLE_COLOR_INVERTED = "#C8C8C8"
var LIST_SUBTITLE_COLOR_PRESSED_INVERTED = "#797979"

var LIST_SUBHEAD_BGCOLOR = "#EDF0F0"
var LIST_SUBHEAD_HEIGHT = 74 * rate
var LIST_SUBHEAD_FONT_COLOR = "#787777"
var LIST_SUBHAED_FONT_SIZE = 24 * rate
var LIST_SUBHEAD_BOTTOM_HEIGHT = 18 * rate
var LIST_SEPARETOR_LINE_HEIGHT = 2 * rate
var LIST_SEPARETOR_LINE_COLOR = "#e8e8e8"
var LIST_SUBHEAD_FONT_FAMILY = DEFAULT_FONT_FAMILY
var LIST_TOP_INTERVAL_HEIGHT = 24 * rate
var LIST_BTOTTOM_INTERVAL_HEIGHT = 24 * rate

var LIST_ICON_MARGIN_LEFT = 20 * rate
var LIST_ICON_MARGIN_RIGHT = 20 * rate
var LIST_ICON_WIDTH = 50 * rate
var LIST_ICON_HEIGHT = 50 * rate
var LIST_RIGHTICON_WIDTH = 22 * rate
var LIST_RIGHTICON_HEIGHT = 34 * rate
var LIST_MAIN_SUB_SEPARATOR = 10 * rate
var LIST_WIFI_RIGHTICON_WIDTH = 50 * rate
var LIST_WIFI_RIGHTICON_HEIGHT = 50 * rate
var LIST_WIFI_STRENTH_ICON_WIDTH = 54 * rate
var LIST_WIFI_STRENTH_ICON_HEIGHT = 46 * rate
var LIST_WIFI_ICON_SEPARATOR = 35 * rate

var SWITCH_MARGIN_TOP = 25 * rate
var SWITCH_MARGIN_BOTTOM = 25 * rate
var SWITCH_MARGIN_RIGHT = 40 * rate
var SWITCH_WIDTH = 150 * rate
var SWITCH_HEIGHT = 80 * rate
var SWITCH_KNOB_WIDTH = 80 * rate
var SWITCH_KNOB_HEIGHT = 80 * rate
var SWITCH_KNOB_CIRCLE_HEIGHT = 70 * rate
var SWITCH_BG_WIDTH = 150 * rate
var SWITCH_BG_HEIGHT = 70 * rate

var TAB_FONT_SIZE = 34 * rate
var TAB_FONT_FAMILY = DEFAULT_FONT_FAMILY
var TAB_BG_WIDTH = 360 * rate
var TAB_BG_HEIGHT = 110 * rate

var BUTTON_RIGHT_WIDTH = 20 * rate
var BUTTON_RIGHT_HEIGHT = 30 * rate

/*page header*/
var PAGE_HEADER_MARGIN_LEFT = 40 * rate
var PAGE_HEADER_HEIGHT = 100 * rate;
var PAGE_HEADER_FONT_COLOR = "#BBBBBB"
var PAGE_HEADER_FONT_SIZE = 36 * rate;
var PAGE_HEADER_FONT_FAMILY = DEFAULT_FONT_FAMILY
var PAGE_HEADER_BACKGROUND = "#FFFFFF"

/*tab button*/
var TAB_TITLE_SIZE = 34 * rate
/*BUTTON_WIDTH*/
var BUTTON_WIDTH = 720 * rate;
var BUTTON_HEIGHT = 110 * rate;
/* Margins */
var INDENT_DEFAULT = 16 * rate;
var CORNER_MARGINS = 40 * rate;
var MARGIN_DEFAULT = 0 * rate;
var MARGIN_XLARGE = 16 * rate;


// ListDelegate
var DEFAULT_FONTFAMILY = DEFAULT_FONT_FAMILY
var LIST_ITEM_MARGIN = 16 * rate
var LIST_ITEM_SPACING = 16 * rate

/* My Label */
var LAB_HEIGHT = 80 * rate;
var LAB_FONT_FAMILY = DEFAULT_FONT_FAMILY
var LAB_FONT_SIZE = 32 * rate
var LAB_COLOR = "#000"
var LAB_COLOR_DISABLE = "gray"
var LAB_SUB_FONT_SIZE = 22 * rate
var LAB_SUB_COLOR = "gray"
var LAB_SUB_COLOR_DISABLE = "lightgray"


/*Wlan list selected*/
var WLAN_ITEM_SELECTED_COLOR = "#339933"
var WLAN_ITEM_UNSELECTED_COLOR = "#CCCCCC"
var WLAN_ITEM_SELECTEDITEM_WIDTH = 10 * rate;
var WLAN_ITEM_SELECTEDITEM_HEIGHT = 70 * rate;

/* Font properties */
var FONT_DEFAULT_SIZE = 32 * rate;
var FONT_LIGHT_SIZE = 30 * rate;

/* TUMBLER properties */
var TUMBLER_COLOR_TEXT = "#FFFFFF";
var TUMBLER_COLOR_LABEL = "#8C8C8C";
var TUMBLER_COLOR = "#000000";
var TUMBLER_OPACITY_FULL = 1.0;
var TUMBLER_OPACITY = 0.4;
var TUMBLER_OPACITY_LOW = 0.1;
var TUMBLER_FLICK_VELOCITY = 700;
var TUMBLER_ROW_HEIGHT = 64 * rate;
var TUMBLER_LABEL_HEIGHT = 54 * rate;
var TUMBLER_MARGIN = 16 * rate;
var TUMBLER_BORDER_MARGIN = 1;
var TUMBLER_WIDTH = 344 * rate;
var TUMBLER_HEIGHT_PORTRAIT = 256 * rate;
var TUMBLER_HEIGHT_LANDSCAPE = 192 * rate;

/* Button styles */
// Normal
var COLOR_BUTTON_FOREGROUND = "#191919"; // Text color
var COLOR_BUTTON_SECONDARY_FOREGROUND = "#8c8c8c"; // Pressed
var COLOR_BUTTON_DISABLED_FOREGROUND = "#b2b2b4"; // Disabled
// Inverted
var COLOR_BUTTON_INVERTED_FOREGROUND = "#FFFFFF";
var COLOR_BUTTON_INVERTED_SECONDARY_FOREGROUND = "#8c8c8c"; // Pressed
var COLOR_BUTTON_INVERTED_DISABLED_FOREGROUND = "#f5f5f5"; // Disabled

var SIZE_BUTTON = 51 * rate;
var SIZE_SMALL_BUTTON = 43 * rate;
var WIDTH_SMALL_BUTTON = 122 * rate;
var WIDTH_TUMBLER_BUTTON = 222 * rate;

var FONT_BOLD_BUTTON = true;

var INFO_BANNER_OPACITY = 0.9
var INFO_BANNER_LETTER_SPACING = -1.2

//--------------------
var TEXT_MAIN_MARGIN_TOP = 30 * rate
var TEXT_MAIN_MARGIN_BOTTOM = 30 * rate
var TEXT_SUB_MARGIN_TOP = 10 * rate
var TEXT_SUB_MARGIN_BOTTOM = 10 * rate
var RIGHT_TEXT_MARGIN_RIGHT = 10 * rate
var RIGHT_TEXT_WIDTH_MAX = 300 * rate

var PROGRESS_BAR_MARGIN_BOTTOM = 40 * rate

/**
** add by zhaoyan for PasswordLock--start
**
**/
var COLOR_BUTTON_NUMBER_NORMAL = "#414141";
var COLOR_BUTTON_NUMBER_PRESSED = "#343636";

var FONT_SIZE_BUTTON_NUMBER = 48 * rate;
var FONT_COLOR_BUTTON_NUMBER = "#ffffff";

var LINE_BORDER_WIDTH_BUTTON_NUMBER = 1;
var LINE_BORDER_HEIGHT_BUTTON_NUMBER = 1;

var FONT_SIZE_ERROR_TEXT = 18 * rate;
var FONT_COLOR_ERROR_TEXT = "#f50c0c";

var FONT_SIZE_INPUT_TEXT = 24 * rate;
var FONT_COLOR_INPUT_TEXT = "#606060";

var FONT_SIZE_FINGER_TIP = 36* rate;
var FONT_SIZE_FINGER_USERTIP = 45* rate;
var FONT_COLOR_FINGER_TIP = "#000000";

var NUMBER_GRIDVIEW_ROW_COUNT = 4;
var NUMBER_GRIDVIEW_COLUMN_COUNT = 3;

var IMAGE_WIDTH_PASSWORD = 50;
var IMAGE_HEIGHT_PASSWORD = 50;
var LENGTH_PASSWORD = 4

var COLOR_BACKGROUND = "#000000"
var COLOR_KEYBACKGROUND = "#ffffff"

var LENGTH_PASSWORD = 4;
var IMAGE_WIDTH_PASSWORD = 30;
var IMAGE_HEIGHT_PASSWORD = 30;
var IMAGE_SPACE_PASSWORD = 10;

var LENGTH_SCREEN = rate * 720;
var HEIGHT_SCREEN = rate * 1280;

var TYPE_SIMPLEPASSWORD = 0;
var TYPE_COMPLEXPASSWORD = 1;

var TOPMAGIN_INPUTTEXT = rate * 100;
var HEIGHT_TEXT = rate * 36;

var FONT_COLOR_INPUT_TEXT = "#000000";
var FONT_SIZE_INPUT_TEXT = rate * 48;


var FONT_SIZE_BACK_BUTTON = rate * 48;
var FONT_SIZE_NUMBER_BUTTON = rate * 48;
var FONT_COLOR_NUMBER_BUTTON = "#ffffff";

var TOPMAGIN_INPUTTEXT = rate * 295;
var BOTTOMMAGIN_INPUTTEXT = rate * 100;

var COUNT_GRIDVIEW_ROW = 4;
var CELLWIDTH_PASSWORD_BUTTON = rate * 240;
var CELLHEIGHT_PASSWORD_BUTTON = rate *250;

var COLOR_BUTTON_NUMBER_NORMAL = "#414141";
var COLOR_BUTTON_NUMBER_PRESSED = "#343636";


var FONT_SIZE_BUTTON_NUMBER = 48 * rate;
var FONT_COLOR_BUTTON_NUMBER = "#ffffff";

var LINE_BORDER_WIDTH_BUTTON_NUMBER = 1;
var LINE_BORDER_HEIGHT_BUTTON_NUMBER = 1;

var FONT_SIZE_ERROR_TEXT = 18 * rate;
var FONT_COLOR_ERROR_TEXT = "#ff0000";

var FONT_SIZE_INPUT_TEXT = 30 * rate;
var FONT_COLOR_INPUT_TEXT = "#ffffff";

var TOPMARGIN_TITLE = rate * 200;
var TOPMARGIN_SUBTITLE = rate * 40;
var TOPMARGIN_LINE = rate * 88;
var TOPMARGIN_ERRORTEXT = rate * 22;
var CHECK_TOPMARGIN_LINE = rate * 174;

var KEYBOARD_HEIGHT = rate * 486;

var FONT_TITLE = rate * 40;
var FONT_SUBTITLE = rate * 26;
var FONT_ERROR = rate * 28;

var COLOR_TITLE = "#ffffff";
var COLOR_SUBTITLE = "#ffffff";
var COLOR_ERROR = "#f84545";

var SCAN_ITEM_H = rate* 100;
var FONT_SIZE_16 = 16 * rate;
var FONT_SIZE_18 = 18 * rate;
var FONT_SIZE_22 = 22 * rate;
var DEVICE_ARCHIVES_LEFT_MARGIN = 40 * rate;
var DEVICE_ARCHIVES_TOP_MARGIN = 14 * rate;
var DEVICE_ARCHIVES_TEXT_TOP_MARGIN = 25 * rate;

var COLOR_DEVICE_ARCHIVES_TEXT = "#666666";
// show check version
var SHOW_CHECK_VERSION_PAGE = "1";
// show version update
var SHOW_VERSION_UPDATE_PAGE = "2";
// clear version page
var CLEAR_VERSION_PAGE = "0";
// show installing
var SHOW_INSTALLING_PAGE = "3";
// show install error
var SHOW_INSTALL_ERROR_PAGE = "4";
// systemMsg
var SYSTEM_MESSAGE_TYPE = "1";
// businessMsg
var BUSINESS_MESSAGE_TYPE = "2";
// download SUCCESS
var  DOWNLOAD_STATUS_SUCCESS = "0";
var  INSTALL_STATUS_SUCCESS = "3";
// Home name IDS
var THEME_NAMEID_INSPECTION = "Inspection";
var THEME_NAMEID_OPERATION = "Operation";
var THEME_NAMEID_INVENTORY = "Inventory";
var THEME_NAMEID_MAINTENANCE = "Maintenance";
var THEME_NAMEID_DEVICE_QUERY = "DeviceQuery";
var THEME_NAMEID_STANDARDIZATION = "Standardization";
var THEME_NAMEID_RECTIFICATION = "Rectification";
var THEME_NAMEID_RECTIFICATION_TASK = "RectificationTasks";
var THEME_NAMEID_RECTIFICATION_CONFIRM = "RectificationConfirm";
var THEME_NAMEID_COMPREHENSIVE_BUSINESS = "ComprehensiveBusiness";
var THEME_NAMEID_DATAREPORT = "DataReport";
var THEME_NAMEID_DAILYTASK = "DailyTask";
var THEME_NAMEID_USER_CARD = "UserCard";
var THEME_NAMEID_SETTINGS = "Settings";
var THEME_NAMEID_MESSAGES = "Messages";
var THEME_NAMEID_SCANTASK = "ScanTask";
var THEME_NAMEID_KNOWLEDGE = "KnowLedge";
var THEME_NAMEID_DEVICE_TASKING = "DeviceTaking";
var THEME_NAMEID_DEVICE_ALLOT = "DeviceAllot";
var THEME_NAMEID_RFID_ASSOCATION = "RFIDAssociation";
var THEME_NAMEID_LOGIN_LOGO = "LoginLogo";
var THEME_NAMEID_LOGIN_TITLE = "LoginTitle";
var THEME_NAMEID_LOGIN_SUB_TITLE = "LoginSubTitle";
var THEME_NAMEID_LOGIN_USER_LEFT_ICON = "LoginUserLeftIcon";
var THEME_NAMEID_LOGIN_USER_RIGHT_ICON = "LoginUserRightIcon";
var THEME_NAMEID_LOGIN_PASSWORD_LEFT_iCON = "LoginPasswrdLeftIcon";
var THEME_NAMEID_LOGIN_BTN = "LoginBtn";
var THEME_NAMEID_LOGOUT_BTN = "LogoutBtn";
var THEME_NAMEID_HOME_LOGO = "HomeLogo";
var MAX_DOWNLOAD_COUNT = 3;

