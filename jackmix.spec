%define distversion %( perl -e 'Creating /home/arnold/programme/jackmix-0.1/jackmix.spec...=\<\>;/(\d+)\.(\d)\.?(\d)?/; print "".(||0)' /etc/*-release)
Name: jackmix
Summary: JackMix -- Mixer for Jack
Version: 0.0.1
Release: %{_vendor}_%{distversion}
Copyright: GPL
Group: X11/KDE/Utilities
Source: http://roederberg.dyndns.org/~arnold/file_share/jackmix/%{name}-%{version}.tar.gz
Packager: Arnold Krille <arnold@arnoldarts.de>
BuildRoot: /tmp/%{name}-%{version}
Prefix: /usr/kde/cvs-head

%description
A long description

%prep
rm -rf $RPM_BUILD_ROOT
%setup -n %{name}-%{version}
CFLAGS="" CXXFLAGS="" ./configure 	--disable-debug --enable-final --prefix=%{prefix}

%build
# Setup for parallel builds
numprocs=1
if [ "" = "0" ]; then
  numprocs=1
fi

make -j

%install
make install-strip DESTDIR=$RPM_BUILD_ROOT

cd 
find . -type d | sed '1,2d;s,^\.,\%attr(-\,root\,root) \%dir ,' > /%{name}-master.list
find . -type f -o -type l | sed 's|^\.||' >> $RPM_BUILD_DIR/%{name}-master.list

%clean
rm -rf $RPM_BUILD_DIR/%{name}-%{version}
rm -rf $RPM_BUILD_DIR/-master.list

%files -f $RPM_BUILD_DIR/%{name}-master.list
