#!/usr/bin/python3
# -*- coding: UTF-8 -*-

# Copyright (C) 2018,Tianjin KYLIN Information Technology Co., Ltd.
#
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

from gi.repository import GLib
import os

def post_feedback(s, url, fb_type, email, description, filename, code):
    #print(s.cookies.get_dict())
    data = {
        "para81": fb_type,
        "para5": "user",
        "para7": email,
        "para12": description,
        "code": code,
        "fdtitle": "在线反馈"
    }
    if not filename:
        files = None
    else:
        files = {
            "para79": ("some_picture.jpg", open(filename, "rb"))
        }
    res = s.post(url, data=data, files=files)
    return res.text

def get_captcha_image(url, s):
    res = s.get(url)
    #s.cookies.update(res.cookies)
    config_dir = GLib.get_user_config_dir()
    icon_path = os.path.join(config_dir, "ukui-menu", 'captcha_img.png')

    with open(icon_path,"wb") as f:
        f.write(res.content)
        f.close()
#    image = Image.open(icon_path)
#    image.show()
#    imgry = image.convert('L')
#    threshold = 140
#    table = []
#    for i in range(256):
#        if i < threshold:
#            table.append(0)
#        else:
#            table.append(1)
#    image = imgry.point(table, '1')
#    captcha_auto = pytesseract.image_to_string(image)
    return icon_path
