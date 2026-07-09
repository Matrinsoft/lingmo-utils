%global majver 1

Name:          lingmo-utils
Version:       1.0.0
Release:       1%{?dist}
Summary:       Lingmo Desktop - Utility library

License:       GPL-2.0-or-later
URL:           https://lingmo.org
Source0:       %{name}-%{version}.tar.gz

BuildRequires: cmake >= 3.21
BuildRequires: ninja-build
BuildRequires: qt6-qtbase-devel >= 6.5

%description
Lingmo Utils provides common utility functions for Lingmo Desktop
components, including structured logging, file operations, XDG
path resolution, and process management.

%package libs
Summary: Shared library for %{name}

%description libs
Shared library providing Lingmo Desktop utility functions.

%package devel
Summary: Development files for %{name}
Requires: %{name}-libs%{?_isa} = %{version}-%{release}
Requires: qt6-qtbase-devel

%description devel
Development headers and CMake integration for Lingmo Utils.

%prep
%setup -q

%build
%cmake -G Ninja \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DBUILD_TESTING=OFF
%cmake_build

%install
%cmake_install

%check
# Tests are run in CI

%files libs
%license LICENSE
%{_libdir}/libLingmoUtils.so.%{majver}*

%files devel
%{_includedir}/LingmoUtils/
%{_libdir}/libLingmoUtils.so
%{_libdir}/cmake/LingmoUtils/

%changelog
* Wed Jul 09 2026 Lingmo OS Team <team@lingmo.org> - 1.0.0-1
- Initial release
