Name:           qt6-kdsingleapplication
Version:        1.0.0
Release:        1
Summary:        KDAB's helper class for Qt6 single-instance policy applications
Source0:        %{name}-%{version}.tar.gz
Source1:        %{name}-%{version}.tar.gz.asc
Source2:        %{name}-rpmlintrc
URL:            https://github.com/KDAB/KDSingleApplication
Group:          System/Libraries
License:        MIT
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Vendor:         Klaralvdalens Datakonsult AB (KDAB)
Packager:       Klaralvdalens Datakonsult AB (KDAB) <info@kdab.com>

BuildRequires: cmake
%if %{defined suse_version}
BuildRequires:  libqt6-qtbase-devel
%endif

%if %{defined fedora}
BuildRequires:  gcc-c++ qt6-qtbase-devel
%endif

%if %{defined rhel}
BuildRequires:  gcc-c++ qt6-qtbase-devel
%endif

%description
KDSingleApplication is a helper class for Qt5 single-instance policy applications
written by KDAB.

Currently the documentation is woefully lacking, but see the examples or tests
for inspiration. Basically it involves:

1. Create a `Q(Core|Gui)Application` object.
2. Create a `KDSingleApplication` object.
3. Check if the current instance is *primary* (or "master") or
   *secondary* (or "slave") by calling `isPrimaryInstance`:
    * the *primary* instance needs to listen from messages coming from the
      secondary instances, by connecting a slot to the `messageReceived` signal;
    * the *secondary* instances can send messages to the primary instance
      by calling `sendMessage`.

Authors:
--------
      Klaralvdalens Datakonsult AB (KDAB) <info@kdab.com>

%define debug_package %{nil}
%global __debug_install_post %{nil}

%package devel
Summary:        Development files for %{name}
Group:          Development/Libraries/C and C++
Requires:       %{name} = %{version}

%description devel
This package contains header files and associated tools and libraries to
develop programs using kdsingleapplication.

%prep
%autosetup

%build
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DKDSingleApplication_QT6=True -DKDSingleApplication_STATIC=True -DCMAKE_BUILD_TYPE=Release
%__make %{?_smp_mflags}

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%install
%make_install

%clean
%__rm -rf "%{buildroot}"

%files
%defattr(-,root,root)
%{_prefix}/share/doc/KDSingleApplication-qt6

%files devel
%defattr(-,root,root,-)
%dir %{_includedir}/kdsingleapplication-qt6
%{_includedir}/kdsingleapplication-qt6/kdsingleapplication/*
%dir %{_libdir}/cmake/KDSingleApplication-qt6
%{_libdir}/cmake/KDSingleApplication-qt6/*
%{_libdir}/libkdsingleapplication-qt6.a
#%{_prefix}/mkspecs/modules/* ECMGeneratePriFile isn't ported to Qt6 yet

%changelog
* Mon Jul 17 2023 Allen Winter <allen.winter@kdab.com> 1.0.0
  1.0.0 final
