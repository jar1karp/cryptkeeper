%if 0%{?git:1}
	%global commit %(git ls-remote https://github.com/jar1karp/cryptkeeper.git refs/heads/master|awk '{print $1}')
%else
	%global commit 80ec579fcec5483bc277ab73960ba7a6e27f1350
%endif

%global today %(date +%Y%m%d)

%global shortcommit %(c=%{commit}; echo ${c:0:7})

Name:           cryptkeeper
Version:        0.9.5_%{today}git%{shortcommit}
Release:        1%{?dist}
Summary:        A Linux system tray applet that manages EncFS encrypted folders

Group:          User Interface/X
License:        GPLv3
URL:            http://tom.noflag.org.uk/cryptkeeper.html
#Source0:        http://tom.noflag.org.uk/cryptkeeper/%{name}-%{version}.tar.gz
Source0:        https://github.com/jar1karp/cryptkeeper/archive/%{commit}/%{name}-%{commit}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  gtk2-devel >= 2.8
BuildRequires:  GConf2-devel automake autoconf desktop-file-utils gettext
BuildRequires:  gettext-autopoint

Requires:       fuse-encfs

%description
A Linux system tray applet that manages EncFS encrypted folders

%prep
%setup -qn %{name}-%{commit}
autoreconf -if

%build
%configure
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
desktop-file-validate $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop
install -dm 755 $RPM_BUILD_ROOT%{_datadir}/man/man1
install -Dpm 644 cryptkeeper.1 $RPM_BUILD_ROOT%{_datadir}/man/man1
%find_lang %{name}

%clean
rm -rf $RPM_BUILD_ROOT


%files -f %{name}.lang
%defattr(-,root,root,-)
%doc COPYING TODO
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png
%{_datadir}/man/man1/%{name}.1.gz
