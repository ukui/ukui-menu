#!/usr/bin/python3
# -*- coding: utf-8 -*-

# Copyright (C) 2018,Tianjin KYLIN Information Technology Co., Ltd.
# Copyright (C) 2006 - 2009 Canonical Ltd.
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the
# Free Software Foundation, Inc.,
# 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.

import subprocess, os, sys, locale

class SystemInfo:
    _os_version = None
    _kernel_info = None
    _uname = None
    _distrorelease = None
    _desktop_env = None
    _default_locale = None
    _arch = None

    def get_os_version(self):
        if self._os_version:
            return self._os_version

        if os.path.exists('/etc/os-release'):
            name = None
            version = None
            with open('/etc/os-release') as f:
                for l in f:
                    if l.startswith('NAME='):
                        name = l.split('=', 1)[1]
                        if name.startswith('"'):
                            name = name[1:-2].strip()
                        if name.endswith('GNU/Linux'):
                            name = name.split()[0:-1]
                    elif l.startswith('VERSION_ID='):
                        version = l.split('=', 1)[1]
                        if version.startswith('"'):
                            version = version[1:-2].strip()
            if name and version:
                self._os_version = (name, version)
                return self._os_version
            else:
                sys.stderr.write('invalid /etc/os-release: Does not contain NAME and VERSION_ID\n')

        # fall back to lsb_release
        p = subprocess.Popen(['lsb_release', '-sir'], stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE)
        (name, version) = p.communicate()[0].decode().strip().replace('\n', ' ').split()
        self._os_version = (name.strip(), version.strip())
        return self._os_version

    def get_system_architecture(self):
        if self._arch:
            return self._arch

        dpkg = subprocess.Popen(['dpkg', '--print-architecture'],
                                stdout=subprocess.PIPE)
        arch = dpkg.communicate()[0].decode().strip()
        assert dpkg.returncode == 0
        assert arch
        return arch

    def get_kernel_info(self):
        if self._kernel_info:
            return self._kernel_info

        self._kernel_info = 'linux-image-' + os.uname()[2]
        return self._kernel_info

    def get_os_info(self):
        if self._uname:
            return self._uname

        u = os.uname()
        self._uname = '%s %s %s' % (u[0], u[2], u[4])
        return self._uname

    def get_distrorelease(self):
        if self._distrorelease:
            return self._distrorelease
        self._distrorelease = '%s %s' % self.get_os_version()
        return self._distrorelease

    def get_default_locale(self):
        if self._default_locale:
            return self._default_locale

        self._default_locale = locale.getdefaultlocale()
        return self._default_locale

    def get_desktop_env(self):
        if self._desktop_env:
            return self._desktop_env
        self._desktop_env = os.environ.get('DESKTOP_SESSION')
        return self._desktop_env

if __name__ == '__main__':
    systeminfo = SystemInfo()
    print("%s; %s; %s; %s; %s; %s" % (systeminfo.get_distrorelease(), systeminfo.get_os_info(), systeminfo.get_kernel_info(),
                                      systeminfo.get_desktop_env(), systeminfo.get_system_architecture(), systeminfo.get_default_locale()))
