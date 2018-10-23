#
# This file is part of user-guide
#
# Copyright (C) 2014 Beijing Yuan Xin Technology Co.,Ltd. All rights reserved.
#
# Authors:
#       Shou Xiaoyun <shouxiaoyun@syberos.com>
#
# This software, including documentation, is protected by copyright controlled
# by Beijing Yuan Xin Technology Co.,Ltd. All rights are reserved.
#


%define appid %{name}
%define _app_system_appdir /usr/apps/%{appid}

Name:           deviceInspection
Version:        0.8.0.w302
Release:        1
Summary:        deviceInspection
Group:          Applications/System
License:        GPL
URL:            www.syberos.net
Source0:        deviceInspection-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-root
Requires:       qt5-qtdeclarative-import-localstorageplugin
Requires:       connman-qt5
Requires:       connman-qt5-declarative
Requires:       syberos-qt
Requires:       syberos-qt-devel
Requires:       libdevman
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(syberos-application)
BuildRequires:  pkgconfig(syberos-application-cache)
BuildRequires:  pkgconfig(Qt5Sql)
BuildRequires:  pkgconfig(Qt5Nfc)
BuildRequires:  pkgconfig(zlib)
BuildRequires:  python
BuildRequires:  pkgconfig(syberos-qt-system)
BuildRequires:  syberos-qt-devel
BuildRequires:  pkgconfig(Qt5Xml)
BuildRequires:  pkgconfig(Qt5Multimedia)
BuildRequires:  openssl-devel
BuildRequires:  qt5-qtconnectivity-qtnfc-devel
BuildRequires:  qt5-qtpositioning
BuildRequires:  qt5-qtpositioning-devel
#BuildRequires:  qt5-qtdeclarative-import-positioning
#BuildRequires:  qt5-plugin-position-geoclue



%description
deviceInspection

%prep
%setup -q -n %{name}-%{version}/inspection


%build

# >> build pre
# << build pre

#%qmake5
%qmake5 TARGET_DIR=%{_app_system_appdir}

make %{?jobs:-j%jobs}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre
#mkdir -p %{buildroot}/usr/lib/systemd/user/user-session.target.wants
#ln -s ../deviceInspection.service %{buildroot}/usr/lib/systemd/user/user-session.target.wants/deviceInspection.service

%qmake5_install
# >> install post
# << install post



#mkdir -p /usr/lib/systemd/user/user-session
#cp ./deviceInspection

%files
%defattr(-,system,system,-)
%{_app_system_appdir}/*
#/usr/lib/systemd/user/user-session.target.wants/deviceInspection.service
# >> files
# << files
