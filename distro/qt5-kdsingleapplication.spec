Name:           qt5-kdsingleapplication
Version:        1.0.0
Release:        1
Summary:        KDAB's helper class for Qt5 single-instance policy applications
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
BuildRequires:  libqt5-qtbase-devel
%endif

%if %{defined fedora}
BuildRequires:  gcc-c++ qt5-qtbase-devel
%endif

%if %{defined rhel}
BuildRequires:  gcc-c++ qt5-qtbase-devel
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
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DKDSingleApplication_STATIC=True
%__make %{?_smp_mflags}

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%install
%make_install

%clean
%__rm -rf "%{buildroot}"

%files
%defattr(-,root,root)
%{_prefix}/share/doc/KDSingleApplication

%files devel
%defattr(-,root,root,-)
%if 0%{?sle_version} >= 150200 && 0%{?is_opensuse}
%{_libdir}/qt5/mkspecs/modules/*
%endif
%if 0%{?suse_version} > 1500
%{_libdir}/qt5/mkspecs/modules/*
%endif
%if 0%{?fedora} > 28
%{_libdir}/qt5/mkspecs/modules/*
%endif
%if %{defined rhel}
%{_libdir}/qt5/mkspecs/modules/*
%endif
%dir %{_includedir}/kdsingleapplication
%{_includedir}/kdsingleapplication/*
%dir %{_libdir}/cmake/KDSingleApplication
%{_libdir}/cmake/KDSingleApplication/*
%{_libdir}/libkdsingleapplication.a

%changelog
* Mon Jul 17 2023 Allen Winter <allen.winter@kdab.com> 1.0.0
  1.0.0 final
