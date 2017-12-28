#!/usr/bin/env python3

# Copyright (C) 2017,Tianjin KYLIN Information Technology Co., Ltd.
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

import ukuimenu
import gi
gi.require_version('Gtk', '3.0')
gi.require_version('Gdk', '3.0')
from gi.repository import GLib, Gio
from gi.repository import Gtk, Gdk, GdkPixbuf
import os
import gettext
import locale
import codecs
import cgi
import shutil
import subprocess

import xml.dom.minidom
import xml.parsers.expat

#i18n
gettext.install("ukui-menu", "/usr/share/locale")

DESKTOP_GROUP = GLib.KEY_FILE_DESKTOP_GROUP
KEY_FILE_FLAGS = GLib.KeyFileFlags.KEEP_COMMENTS | GLib.KeyFileFlags.KEEP_TRANSLATIONS
    
def fillKeyFile(keyfile, items):
    for key, item in items.items():
        if item is None:
            continue

        if isinstance(item, bool):
            keyfile.set_boolean(DESKTOP_GROUP, key, item)
        elif isinstance(item, Sequence):
            keyfile.set_string_list(DESKTOP_GROUP, key, item)
        elif isinstance(item, basestring):
            keyfile.set_string(DESKTOP_GROUP, key, item)

def getUniqueFileId(name, extension):
    append = 0
    while 1:
        if append == 0:
            filename = name + extension
        else:
            filename = name + '-' + str(append) + extension
        if extension == '.desktop':
            path = getUserItemPath()
            if not os.path.isfile(os.path.join(path, filename)) and not getItemPath(filename):
                break
        elif extension == '.directory':
            path = getUserDirectoryPath()
            if not os.path.isfile(os.path.join(path, filename)) and not getDirectoryPath(filename):
                break
        append += 1
    return filename

def getUniqueRedoFile(filepath):
    append = 0
    while 1:
        new_filepath = filepath + '.redo-' + str(append)
        if not os.path.isfile(new_filepath):
            break
        else:
            append += 1
    return new_filepath

def getUniqueUndoFile(filepath):
    filename, extension = os.path.split(filepath)[1].split('.')
    append = 0
    while 1:
        if extension == 'desktop':
            path = getUserItemPath()
        elif extension == 'directory':
            path = getUserDirectoryPath()
        elif extension == 'menu':
            path = getUserMenuPath()
        new_filepath = os.path.join(path, filename + '.' + extension + '.undo-' + str(append))
        if not os.path.isfile(new_filepath):
            break
        else:
            append += 1
    return new_filepath

def getItemPath(file_id):
    for path in GLib.get_system_data_dirs():
        file_path = os.path.join(path, 'applications', file_id)
    if os.path.isfile(file_path):
        return file_path
    return None

def getUserItemPath():
    item_dir = os.path.join(GLib.get_user_data_dir(), 'applications')
    if not os.path.isdir(item_dir):
        os.makedirs(item_dir)
    return item_dir

def getDirectoryPath(file_id):
    for path in GLib.get_system_data_dirs():
        file_path = os.path.join(path, 'desktop-directories', file_id)
        if os.path.isfile(file_path):
            return file_path
    return None

def getUserMenuPath():
    menu_dir = os.path.join(GLib.get_user_config_dir(), 'menus')
    if not os.path.isdir(menu_dir):
        os.makedirs(menu_dir)
    return menu_dir

def getUserDirectoryPath():
    menu_dir = os.path.join(GLib.get_user_data_dir(), 'desktop-directories')
    if not os.path.isdir(menu_dir):
        os.makedirs(menu_dir)
    return menu_dir
    
def getSystemMenuPath(file_id):
    for path in GLib.get_system_config_dirs():
        file_path = os.path.join(path, 'menus', file_id)
        if os.path.isfile(file_path):
            return file_path
    return None

def getUserMenuXml(tree):
    system_file = getSystemMenuPath(tree.get_menu_file().decode('utf-8'))
    name = tree.get_root_directory().get_menu_id().decode('utf-8')
    menu_xml = "<!DOCTYPE Menu PUBLIC '-//freedesktop//DTD Menu 1.0//EN' 'http://standards.freedesktop.org/menu-spec/menu-1.0.dtd'>\n"
    menu_xml += "<Menu>\n  <Name>" + name + "</Name>\n  "
    menu_xml += "<MergeFile type=\"parent\">" + system_file +       "</MergeFile>\n</Menu>\n"
    return menu_xml


def getIcon(item):
    pixbuf, path = None, None
    if item is None:
        return None
    if isinstance(item, str):
        iconName = item
    else:
        if item.get_icon() == None:
            return None
        iconName = item.get_icon().decode('utf-8')
    if iconName and not '/' in iconName and iconName[-3:] in ('png', 'svg', 'xpm'):
        iconName = iconName[:-4]
    icon_theme = Gtk.IconTheme.get_default()
    try:
        pixbuf = icon_theme.load_icon(iconName, 24, 0)
        path = icon_theme.lookup_icon(iconName, 24, 0).get_filename()
    except:
        if iconName and '/' in iconName:
            try:
                pixbuf = GdkPixbuf.Pixbuf.new_from_file_at_size(iconName, 24, 24)
                path = iconName
            except:
                pass
        if pixbuf is None:
            if item.get_type() == ukuimenu.TYPE_DIRECTORY:
                iconName = 'ukui-fs-directory'
            elif item.get_type() == ukuimenu.TYPE_ENTRY:
                iconName = 'application-default-icon'
            try:
                pixbuf = icon_theme.load_icon(iconName, 24, 0)
                path = icon_theme.lookup_icon(iconName, 24, 0).get_filename()
            except:
                return None
    if pixbuf is None:
        return None
    if pixbuf.get_width() != 24 or pixbuf.get_height() != 24:
        pixbuf = pixbuf.scale_simple(24, 24, GdkPixbuf.InterpType.HYPER)
    return pixbuf

def removeWhitespaceNodes(node):
    remove_list = []
    for child in node.childNodes:
        if child.nodeType == xml.dom.minidom.Node.TEXT_NODE:
            child.data = child.data.strip()
            if not child.data.strip():
                remove_list.append(child)
            elif child.hasChildNodes():
                removeWhitespaceNodes(child)
    for node in remove_list:
        node.parentNode.removeChild(node)




class Menu:
    tree = None
    visible_tree = None
    path = None
    dom = None

class MenuEditor:
    def __init__(self):
        self.locale = locale.getdefaultlocale()[0]
        self.__loadMenus()
        self.__undo = []
        self.__redo = []

    def __loadMenus(self):
        self.applications = Menu()
        self.applications.tree = ukuimenu.lookup_tree('ukui-applications.menu', ukuimenu.FLAGS_SHOW_EMPTY|ukuimenu.FLAGS_INCLUDE_EXCLUDED|ukuimenu.FLAGS_INCLUDE_NODISPLAY|ukuimenu.FLAGS_SHOW_ALL_SEPARATORS)
        self.applications.visible_tree = ukuimenu.lookup_tree('ukui-applications.menu')
        self.applications.tree.set_sort_key(ukuimenu.SORT_DISPLAY_NAME)
        self.applications.visible_tree.set_sort_key(ukuimenu.SORT_DISPLAY_NAME)
        self.applications.path = os.path.join(getUserMenuPath(), self.applications.tree.get_menu_file().decode('utf-8'))
        try:
            self.applications.dom = xml.dom.minidom.parse(self.applications.path)
        except (IOError, xml.parsers.expat.ExpatError):
            self.applications.dom = xml.dom.minidom.parseString(getUserMenuXml(self.applications.tree))
        removeWhitespaceNodes(self.applications.dom)

        self.settings = Menu()
        self.settings.tree = ukuimenu.lookup_tree('ukui-settings.menu', ukuimenu.FLAGS_SHOW_EMPTY|ukuimenu.FLAGS_INCLUDE_EXCLUDED|ukuimenu.FLAGS_INCLUDE_NODISPLAY|ukuimenu.FLAGS_SHOW_ALL_SEPARATORS)
        self.settings.visible_tree = ukuimenu.lookup_tree('ukui-settings.menu')
        self.settings.tree.set_sort_key(ukuimenu.SORT_DISPLAY_NAME)
        self.settings.visible_tree.set_sort_key(ukuimenu.SORT_DISPLAY_NAME)
        self.settings.path = os.path.join(getUserMenuPath(), self.settings.tree.get_menu_file().decode('utf-8'))
        try:
            self.settings.dom = xml.dom.minidom.parse(self.settings.path)
        except (IOError, xml.parsers.expat.ExpatError):
            self.settings.dom = xml.dom.minidom.parseString(getUserMenuXml(self.settings.tree))
        removeWhitespaceNodes(self.settings.dom)

        self.save(True)

    def __isVisible(self, item):
        if item.get_type() == ukuimenu.TYPE_ENTRY:
            return not (item.get_is_excluded() or item.get_is_nodisplay())
        menu = self.__getMenu(item)
        if menu == self.applications:
            root = self.applications.visible_tree.get_root_directory()
        elif menu == self.settings:
            root = self.settings.visible_tree.get_root_directory()
        if item.get_type() == ukuimenu.TYPE_DIRECTORY:
            if self.__findMenu(item.get_menu_id(), root) is None:
                return False
        return True

    def __addUndo(self, items):
        self.__undo.append([])
        for item in items:
            if isinstance(item, Menu):
                file_path = item.path
            elif isinstance(item, tuple):
                if item[0] == 'Item':
                    file_path = os.path.join(getUserItemPath(), item[1])
                    if not os.path.isfile(file_path):
                        file_path = getItemPath(item[1])
                elif item[0] == 'Menu':
                    file_path = os.path.join(getUserDirectoryPath(), item[1])
                    if not os.path.isfile(file_path):
                        file_path = getDirectoryPath(item[1])
                else:
                    continue
            elif item.get_type() == ukuimenu.TYPE_DIRECTORY:
                if item.get_desktop_file_path() is None:
                    continue
                file_path = os.path.join(getUserDirectoryPath(), os.path.split(item.get_desktop_file_path().decode('utf-8'))[1])
                if not os.path.isfile(file_path):
                    file_path = item.get_desktop_file_path().decode('utf-8')
            elif item.get_type() == ukuimenu.TYPE_ENTRY:
                file_path = os.path.join(getUserItemPath(), item.get_desktop_file_id().decode('utf-8'))
                if not os.path.isfile(file_path):
                    file_path = item.get_desktop_file_path().decode('utf-8')
            else:
                continue
            with codecs.open(file_path, 'r', 'utf-8') as f:
                data = f.read()
            undo_path = getUniqueUndoFile(file_path)
            with codecs.open(undo_path, 'w', 'utf-8') as f:
                f.write(data)
            self.__undo[-1].append(undo_path)

    def __getPath(self, menu):
        names = []
        current = menu
        while current is not None:
            names.append(current.get_menu_id())
            current = current.get_parent()

        # XXX - don't append root menu name, mozo doesn't
        # expect it. look into this more.
        names.pop(-1)
        return names[::-1]

    def __getXmlMenuPart(self, element, name):
        for node in self.__getXmlNodesByName('Menu', element):
            for child in self.__getXmlNodesByName('Name', node):
                if child.childNodes[0].nodeValue == name:
                    return node
        return None

    def __getXmlMenu(self, path, element, dom):
        for name in path:
            found = self.__getXmlMenuPart(element, name)
            if found is not None:
                element = found
            else:
                element = self.__addXmlMenuElement(element, name, dom)
        return element

    def __addXmlMenuElement(self, element, name, dom):
        if isinstance(name, bytes):
            name = name.decode('utf-8')

        node = dom.createElement('Menu')
        self.__addXmlTextElement(node, 'Name', name, dom)
        return element.appendChild(node)

    def __addXmlTextElement(self, element, name, text, dom):
        if isinstance(name, bytes):
            name = name.decode('utf-8')
        if isinstance(text, bytes):
            text = text.decode('utf-8')

        for temp in element.childNodes:
            if temp.nodeName == name:
                if temp.childNodes[0].nodeValue == text:
                    return
        node = dom.createElement(name)
        text = dom.createTextNode(text)
        node.appendChild(text)
        return element.appendChild(node)

    def __addXmlFilename(self, element, dom, filename, type = 'Include'):
        if isinstance(filename, bytes):
            filename = filename.decode('utf-8')

        # remove old filenames
        for node in self.__getXmlNodesByName(['Include', 'Exclude'], element):
            if node.childNodes[0].nodeName == 'Filename' and node.childNodes[0].childNodes[0].nodeValue == filename:
                element.removeChild(node)

        # add new filename
        node = dom.createElement(type)
        node.appendChild(self.__addXmlTextElement(node, 'Filename', filename, dom))
        return element.appendChild(node)

    def __addDeleted(self, element, dom):
        node = dom.createElement('Deleted')
        return element.appendChild(node)

    def __makeKeyFile(self, file_path, kwargs):
        if 'KeyFile' in kwargs:
            return kwargs['KeyFile']

        keyfile = GLib.KeyFile()

        if file_path is not None:
            keyfile.load_from_file(file_path, KEY_FILE_FLAGS)

        fillKeyFile(keyfile, kwargs)
        return keyfile

    def __writeItem(self, item, **kwargs):
        if item is not None:
            file_path = item.get_desktop_file_path().decode('utf-8')
        else:
            file_path = None

        keyfile = self.__makeKeyFile(file_path, kwargs)

        if item is not None:
            file_id = item.get_desktop_file_id().decode('utf-8')
        else:
            file_id = getUniqueFileId(keyfile.get_string(DESKTOP_GROUP, 'Name'), '.desktop')

        contents, length = keyfile.to_data()

        with open(os.path.join(getUserItemPath(), file_id), 'w') as f:
            f.write(contents)
        return file_id

    def __writeMenu(self, menu, **kwargs):
        if menu is not None:
            file_id = os.path.split(menu.get_desktop_file_path().decode('utf-8'))[1]
            file_path = menu.get_desktop_file_path().decode('utf-8')
            keyfile = GLib.KeyFile()
            keyfile.load_from_file(file_path, KEY_FILE_FLAGS)
        elif menu is None and 'Name' not in kwargs:
            raise Exception('New menus need a name')
        else:
            file_id = getUniqueFileId(kwargs['Name'], '.directory')
            keyfile = GLib.KeyFile()

        fillKeyFile(keyfile, kwargs)
        
        contents, length = keyfile.to_data()

        with open(os.path.join(getUserDirectoryPath(), file_id), 'w') as f:
            f.write(contents)
        return file_id

    def __getXmlNodesByName(self, name, element):
        for child in element.childNodes:
            if child.nodeType == xml.dom.Node.ELEMENT_NODE:
                if isinstance(name, str) and child.nodeName == name:
                    yield child
                elif isinstance(name, list) or isinstance(name, tuple):
                    if child.nodeName in name:
                        yield child

    def __addXmlMove(self, element, old, new, dom):
        if not self.__undoMoves(element, old, new, dom):
            node = dom.createElement('Move')
            node.appendChild(self.__addXmlTextElement(node, 'Old', old, dom))
            node.appendChild(self.__addXmlTextElement(node, 'New', new, dom))
            #are parsed in reverse order, need to put at the beginning
            return element.insertBefore(node, element.firstChild)

    def __addXmlLayout(self, element, layout, dom):
        # remove old layout
        for node in self.__getXmlNodesByName('Layout', element):
            element.removeChild(node)
            
        # add new layout
        node = dom.createElement('Layout')
        for order in layout.order:
            if order[0] == 'Separator':
                child = dom.createElement('Separator')
                node.appendChild(child)
            elif order[0] == 'Filename':
                child = self.__addXmlTextElement(node, 'Filename', order[1], dom)
            elif order[0] == 'Menuname':
                child = self.__addXmlTextElement(node, 'Menuname', order[1], dom)
            elif order[0] == 'Merge':
                child = dom.createElement('Merge')
                child.setAttribute('type', order[1])
                node.appendChild(child)
        return element.appendChild(node)

    def __addXmlDefaultLayout(self, element, dom):
        # remove old default layout
        for node in self.__getXmlNodesByName('DefaultLayout', element):
            element.removeChild(node)

        # add new layout
        node = dom.createElement('DefaultLayout')
        node.setAttribute('inline', 'false')
        return element.appendChild(node)

    def __createLayout(self, items):
        layout = Layout()
        layout.order = []

        layout.order.append(['Merge', 'menus'])
        for item in items:
            if isinstance(item, tuple):
                if item[0] == 'Separator':
                    layout.parseSeparator()
                elif item[0] == 'Menu':
                    layout.parseMenuname(item[1])
                elif item[0] == 'Item':
                    layout.parseFilename(item[1])
            elif item.get_type() == ukuimenu.TYPE_DIRECTORY:
                layout.parseMenuname(item.get_menu_id())
            elif item.get_type() == ukuimenu.TYPE_ENTRY:
                layout.parseFilename(item.get_desktop_file_id())
            elif item.get_type() == ukuimenu.TYPE_SEPARATOR:
                layout.parseSeparator()
        layout.order.append(['Merge', 'files'])
        return layout

    def __addItem(self, parent, file_id, dom):
        xml_parent = self.__getXmlMenu(self.__getPath(parent), dom.documentElement, dom)
        self.__addXmlFilename(xml_parent, dom, file_id, 'Include')

    def __positionItem(self, parent, item, before=None, after=None):
        if after:
            index = parent.contents.index(after) + 1
        elif before:
            index = parent.contents.index(before)
        else:
            # append the item to the list
            index = len(parent.contents)
        contents = parent.contents
        #if this is a move to a new parent you can't remove the item
        if item in contents:
            # decrease the destination index, if we shorten the list
            if (before and (contents.index(item) < index)) \
                    or (after and (contents.index(item) < index - 1)):
                        index -= 1
            contents.remove(item)
        contents.insert(index, item)
        layout = self.__createLayout(contents)
        dom = self.__getMenu(parent).dom
        menu_xml = self.__getXmlMenu(self.__getPath(parent), dom.documentElement, dom)
        self.__addXmlLayout(menu_xml, layout, dom)

    def __undoMoves(self, element, old, new, dom):
        nodes = []
        matches = []
        original_old = old
        final_old = old
        #get all <Move> elements
        for node in self.__getXmlNodesByName(['Move'], element):
            nodes.insert(0, node)
        #if the <New> matches our old parent we've found a stage to undo
        for node in nodes:
            xml_old = node.getElementsByTagName('Old')[0]
            xml_new = node.getElementsByTagName('New')[0]
            if xml_new.childNodes[0].nodeValue == old:
                matches.append(node)
                #we should end up with this path when completed
                final_old = xml_old.childNodes[0].nodeValue
        #undoing <Move>s
        for node in matches:
            element.removeChild(node)
        if len(matches) > 0:
            for node in nodes:
                xml_old = node.getElementsByTagName('Old')[0]
                xml_new = node.getElementsByTagName('New')[0]
                path = os.path.split(xml_new.childNodes[0].nodeValue)
                if path[0] == original_old:
                    element.removeChild(node)
                    for node in dom.getElementsByTagName('Menu'):
                        name_node = node.getElementsByTagName('Name')[0]
                        name = name_node.childNodes[0].nodeValue
                        if name == os.path.split(new)[1]:
                            #copy app and dir directory info from old <Menu>
                            root_path = dom.getElementsByTagName('Menu')[0].getElementsByTagName('Name')[0].childNodes[0].nodeValue
                            xml_menu = self.__getXmlMenu(root_path + '/' + new, dom.documentElement, dom)
                            for app_dir in node.getElementsByTagName('AppDir'):
                                xml_menu.appendChild(app_dir)
                            for dir_dir in node.getElementsByTagName('DirectoryDir'):
                                xml_menu.appendChild(dir_dir)
                            parent = node.parentNode
                            parent.removeChild(node)
                    node = dom.createElement('Move')
                    node.appendChild(self.__addXmlTextElement(node, 'Old', xml_old.childNodes[0].nodeValue, dom))
                    node.appendChild(self.__addXmlTextElement(node, 'New', os.path.join(new, path[1]), dom))
                    element.appendChild(node)
            if final_old == new:
                return True
            node = dom.createElement('Move')
            node.appendChild(self.__addXmlTextElement(node, 'Old', final_old, dom))
            node.appendChild(self.__addXmlTextElement(node, 'New', new, dom))
            return element.appendChild(node)

    def __getMenu(self, item):
        root = item.get_parent()
        if not root:
            #already at the top
            root = item
        else:
            while True:
                if root.get_parent():
                    root = root.get_parent()
                else:
                    break
        if root.get_menu_id() == self.applications.tree.get_root_directory().get_menu_id():
            return self.applications
        return self.settings

    def __findMenu(self, menu_id, parent=None):
        if parent is None:
            menu = self.__findMenu(menu_id, self.applications.tree.root)
            if menu is not None:
                return menu
            else:
                return self.__findMenu(menu_id, self.settings.tree.root)
        if menu_id == self.applications.tree.get_root_directory().get_menu_id():
            return self.applications.tree.get_root_directory()
        if menu_id == self.settings.tree.get_root_directory().get_menu_id():
            return self.settings.tree.get_root_directory()
        for item in parent.get_contents():
            if item.get_type() == ukuimenu.TYPE_DIRECTORY:
                if item.get_menu_id() == menu_id:
                    return item
                menu = self.__findMenu(menu_id, item)
                if menu is not None:
                    return menu

    def save(self, from_loading=False):
        for menu in ('applications', 'settings'):
            with codecs.open(getattr(self, menu).path, 'w', 'utf-8') as f:
                f.write(getattr(self, menu).dom.toprettyxml())
        if not from_loading:
            self.__loadMenus()

    def quit(self):
        for file_name in os.listdir(self.getUserItemPath()):
            if file_name[-6:-2] in ('redo', 'undo'):
                file_path = os.path.join(self.getUserItemPath(), file_name)
                os.unlink(file_path)
        for file_name in os.listdir(getUserDirectoryPath()):
            if file_name[-6:-2] in ('redo', 'undo'):
                file_path = os.path.join(getUserDirectoryPath(), file_name)
                os.unlink(file_path)
        for file_name in os.listdir(getUserMenuPath()):
            if file_name[-6:-2] in ('redo', 'undo'):
                file_path = os.path.join(getUserMenuPath(), file_name)
                os.unlink(file_path)

    def revert(self):
        for name in ('applications', 'settings'):
            menu = getattr(self, name)
            self.revertTree(menu.tree.root)
            path = os.path.join(getUserMenuPath(), menu.tree.get_menu_file())
            try:
                os.unlink(path)
            except OSError:
                pass
            #reload DOM for each menu
            try:
                menu.dom = xml.dom.minidom.parse(menu.path)
            except (IOError, xml.parsers.expat.ExpatError):
                menu.dom = xml.dom.minidom.parseString(getUserMenuXml(menu.tree))
            removeWhitespaceNodes(menu.dom)
        #reset undo/redo, no way to recover from this
        self.__undo, self.__redo = [], []
        self.save()

    def revertTree(self, menu):
        for child in menu.get_contents():
            if child.get_type() == ukuimenu.TYPE_DIRECTORY:
                self.revertTree(child)
            elif child.get_type() == ukuimenu.TYPE_ENTRY:
                self.revertItem(child)
        self.revertMenu(menu)

    def revertItem(self, item):
        if not self.canRevert(item):
            return
        self.__addUndo([item,])
        try:
            os.remove(item.get_desktop_file_path())
        except OSError:
            pass
        self.save()

    def revertMenu(self, menu):
        if not self.canRevert(menu):
            return
        #wtf happened here? oh well, just bail
        if not menu.get_desktop_file_path():
            return
        self.__addUndo([menu,])
        file_id = os.path.split(menu.get_desktop_file_path())[1]
        path = os.path.join(getUserDirectoryPath(), file_id)
        try:
            os.remove(path)
        except OSError:
            pass
        self.save()

    def undo(self):
        if len(self.__undo) == 0:
            return
        files = self.__undo.pop()
        redo = []
        for file_path in files:
            new_path = file_path.rsplit('.', 1)[0]
            redo_path = getUniqueRedoFile(new_path)

            f_file_path = codecs.open(new_path, 'r', 'utf-8')
            f_new_path = codecs.open(new_path, 'rw', 'utf-8')
            f_redo_path = codecs.open(redo_path, 'rw', 'utf-8')

            data = f_new_path.read()
            f_redo_path.write(data)
            data = f_file_path.read()
            f_new_path.write(data)

            f_file_path.close()
            f_new_path.close()
            f_redo_path.close()

            os.unlink(file_path)
            redo.append(redo_path)
        # reload DOM to make changes stick
        for name in ('applications', 'settings'):
            menu = getattr(self, name)
            try:
                menu.dom = xml.dom.minidom.parse(menu.path)
            except (IOError, xml.parsers.expat.ExpatError):
                menu.dom = xml.dom.minidom.parseString(getUserMenuXml(menu.tree))
            removeWhitespaceNodes(menu.dom)
        self.__redo.append(redo)

    def redo(self):
        if len(self.__redo) == 0:
            return
        files = self.__redo.pop()
        undo = []
        for file_path in files:
            new_path = file_path.rsplit('.', 1)[0]
            undo_path = getUniqueUndoFile(new_path)
            f_file_path = codecs.open(new_path, 'r', 'utf-8')
            f_new_path = codecs.open(new_path, 'rw', 'utf-8')
            f_undo_path = codecs.open(undo_path, 'rw', 'utf-8')
            data = f_new_path.read()
            f_undo_path.write(data)
            data = f_file_path.read()
            f_new_path.write(data)
            os.unlink(file_path)
            undo.append(undo_path)
            f_file_path.close()
            f_new_path.close()
            f_undo_path.close()
        #reload DOM to make changes stick
        for name in ('applications', 'settings'):
            menu = getattr(self, name)
            try:
                menu.dom = xml.dom.minidom.parse(menu.path)
            except (IOError, xml.parsers.expat.ExpatError):
                menu.dom = xml.dom.minidom.parseString(getUserMenuXml(menu.tree))
            removeWhitespaceNodes(menu.dom)
        self.__undo.append(undo)

    def getMenus(self, parent=None):
        if parent is None:
            yield self.applications.tree.get_root_directory()
            yield self.settings.tree.get_root_directory()
        else:
            for menu in parent.get_contents():
                if menu.get_type() == ukuimenu.TYPE_DIRECTORY:
                    yield (menu, self.__isVisible(menu))

    def getItems(self, menu):
        for item in menu.get_contents():
            if item.get_type() == ukuimenu.TYPE_SEPARATOR:
                yield (item, True)
            else:
                if item.get_type() == ukuimenu.TYPE_ENTRY and item.get_desktop_file_id().decode('utf-8')[-19:] == '-usercustom.desktop':
                    continue
                yield (item, self.__isVisible(item))

    def canRevert(self, item):
        if item.get_type() == ukuimenu.TYPE_ENTRY:
            if getItemPath(item.get_desktop_file_id().decode('utf-8')) is not None:
                path = getUserItemPath()
                if os.path.isfile(os.path.join(path, item.get_desktop_file_id().decode('utf-8'))):
                    return True
        elif item.get_type() == ukuimenu.TYPE_DIRECTORY:
            if item.get_desktop_file_path():
                file_id = os.path.split(item.get_desktop_file_path().decode('utf-8'))[1]
            else:
                file_id = item.get_menu_id().decode('utf-8') + '.directory'
            if getDirectoryPath(file_id) is not None:
                path = getUserDirectoryPath()
                if os.path.isfile(os.path.join(path, file_id)):
                    return True
        return False

    def setVisible(self, item, visible):
        dom = self.__getMenu(item).dom
        if item.get_type() == ukuimenu.TYPE_ENTRY:
            self.__addUndo([self.__getMenu(item), item])
            menu_xml = self.__getXmlMenu(self.__getPath(item.get_parent()), dom.documentElement, dom)
            if visible:
                self.__addXmlFilename(menu_xml, dom, item.get_desktop_file_id().decode('utf-8'), 'Include')
                self.__writeItem(item, NoDisplay=False)
            else:
                self.__addXmlFilename(menu_xml, dom, item.get_desktop_file_id().decode('utf-8'), 'Exclude')
            self.__addXmlTextElement(menu_xml, 'AppDir', getUserItemPath(), dom)
        elif item.get_type() == ukuimenu.TYPE_DIRECTORY:
            self.__addUndo([self.__getMenu(item), item])
            #don't mess with it if it's empty
            if len(item.get_contents()) == 0:
                msg = _("\nThe current system is not installed [%s] software, can not display the classification!") % (item.get_name().decode('utf-8'))
                md = Gtk.MessageDialog(None, 0, Gtk.MessageType.WARNING, Gtk.ButtonsType.OK, msg)
                md.run()
                md.destroy()
                return None
            menu_xml = self.__getXmlMenu(self.__getPath(item), dom.documentElement, dom)
            for node in self.__getXmlNodesByName(['Deleted', 'NotDeleted'], menu_xml):
                node.parentNode.removeChild(node)
            self.__writeMenu(item, NoDisplay=not visible)
            self.__addXmlTextElement(menu_xml, 'DirectoryDir', getUserDirectoryPath(), dom)
        self.save()

class Layout:
    def __init__(self, node=None):
        self.order = []

    def parseMenuname(self, value):
        self.order.append(['Menuname', value])

    def parseSeparator(self):
        self.order.append(['Separator'])

    def parseFilename(self, value):
        self.order.append(['Filename', value])

    def parseMerge(self, merge_type='all'):
        self.order.append(['Merge', merge_type])




class MenuEditMainWindow:
    timer = None
    #hack to make editing menu properties work
    allow_update = True
    #drag-and-drop stuff
    dnd_items = [('MOZO_ITEM_ROW', Gtk.TargetFlags.SAME_APP, 0), ('text/plain', 0, 1)]
    dnd_menus = [('MOZO_MENU_ROW', Gtk.TargetFlags.SAME_APP, 0)]
    dnd_both = [dnd_items[0],] + dnd_menus
    drag_data = None
    edit_pool = []

    def __init__(self):
        self.file_path = "./"
        self.editor = MenuEditor()
        self.tree = Gtk.Builder()
        self.tree.add_from_file(os.path.join('/', 'usr', 'share', 'ukui-menu', 'menueditor.ui'))
        self.tree.connect_signals(self)
        self.setupMenuTree()
        self.setupItemTree()
        self.tree.get_object('mainwindow').set_icon_from_file('/usr/share/ukui-menu/icons/start.png')
        self.tree.get_object('edit_delete').set_sensitive(False)
        self.tree.get_object('edit_revert_to_original').set_sensitive(False)
        self.tree.get_object('edit_properties').set_sensitive(False)
        self.tree.get_object('move_up_button').set_sensitive(False)
        self.tree.get_object('move_down_button').set_sensitive(False)
        self.tree.get_object('new_separator_button').set_sensitive(False)
        self.tree.get_object('properties_button').set_sensitive(False)
        self.tree.get_object('delete_button').set_sensitive(False)
        self.tree.get_object('edit_revert_to_original').set_label(_("Revert to Original"))
        self.tree.get_object('mainwindow').set_title(_("Edit Category Menu"))
        self.tree.get_object('revert_button').set_tooltip_text(_("Restore the default menu layout"))
        self.tree.get_object('label18').set_text(_("New Menu"))
        self.tree.get_object('label19').set_text(_("New Item"))
        self.tree.get_object('label20').set_text(_("Menus:"))
        self.tree.get_object('label21').set_text(_("Items:"))
        self.tree.get_object('label22').set_text(_("Move Up"))
        self.tree.get_object('label23').set_text(_("Move Down"))
        self.tree.get_object('label24').set_text(_("Revert all menus to original settings?"))
        self.tree.get_object('revertdialog').set_title(_("Revert Changes?"))
        self.tree.get_object('new_separator_button').set_label(_("New Separator"))
        accelgroup = Gtk.AccelGroup()
        keyval, modifier = Gtk.accelerator_parse('<Ctrl>Z')
        accelgroup.connect(keyval, modifier, Gtk.AccelFlags.VISIBLE, self.on_mainwindow_undo)
        keyval, modifier = Gtk.accelerator_parse('<Ctrl><Shift>Z')
        accelgroup.connect(keyval, modifier, Gtk.AccelFlags.VISIBLE, self.on_mainwindow_redo)
        keyval, modifier = Gtk.accelerator_parse('F1')
        accelgroup.connect(keyval, modifier, Gtk.AccelFlags.VISIBLE, self.on_help_button_clicked)
        self.tree.get_object('mainwindow').add_accel_group(accelgroup)

    def run(self):
        self.loadMenus()
        self.editor.applications.tree.add_monitor(self.menuChanged, None)
        self.editor.settings.tree.add_monitor(self.menuChanged, None)
        self.tree.get_object('mainwindow').show_all()

    def menuChanged(self, *a):
        if self.timer:
            GLib.Source.remove(self.timer)
            self.timer = None
        self.timer = GLib.timeout_add(3, self.loadUpdates)

    def loadUpdates(self):
        if not self.allow_update:
            self.timer = None
            return False
        menu_tree = self.tree.get_object('menu_tree')
        item_tree = self.tree.get_object('item_tree')
        items, iter = item_tree.get_selection().get_selected()
        update_items = False
        item_id, separator_path = None, None
        if iter:
            update_items = True
            if items[iter][3].get_type() == ukuimenu.TYPE_DIRECTORY:
                item_id = os.path.split(items[iter][3].get_desktop_file_path())[1]
                update_items = True
            elif items[iter][3].get_type() == ukuimenu.TYPE_ENTRY:
                item_id = items[iter][3].get_desktop_file_id()
                update_items = True
            elif items[iter][3].get_type() == ukuimenu.TYPE_SEPARATOR:
                item_id = items.get_path(iter).to_string()
                update_items = True
        menus, iter = menu_tree.get_selection().get_selected()
        update_menus = False
        menu_id = None
        if iter:
            if menus[iter][2].get_desktop_file_path():
                menu_id = os.path.split(menus[iter][2].get_desktop_file_path())[1]
            else:
                menu_id = menus[iter][2].get_menu_id()
            update_menus = True
        self.loadMenus()
        #find current menu in new tree
        if update_menus:
            menu_tree.get_model().foreach(self.findMenu, menu_id)
            menus, iter = menu_tree.get_selection().get_selected()
            if iter:
                self.on_menu_tree_cursor_changed(menu_tree)
        #find current item in new list
        if update_items:
            i = 0
            for item in item_tree.get_model():
                found = False
                if item[3].get_type() == ukuimenu.TYPE_ENTRY and item[3].get_desktop_file_id() == item_id:
                    found = True
                if item[3].get_type() == ukuimenu.TYPE_DIRECTORY and item[3].get_desktop_file_path():
                    if os.path.split(item[3].get_desktop_file_path())[1] == item_id:
                        found = True
                if item[3].get_type() == ukuimenu.TYPE_SEPARATOR:
                    if not isinstance(item_id, tuple):
                        #we may not skip the increment via "continue"
                        i += 1
                        continue
                    #separators have no id, have to find them manually
                    #probably won't work with two separators together
                    if (item_id[0] - 1,) == (i,):
                        found = True
                    elif (item_id[0] + 1,) == (i,):
                        found = True
                    elif (item_id[0],) == (i,):
                        found = True
                if found:
                    item_tree.get_selection().select_path((i,))
                    self.on_item_tree_cursor_changed(item_tree)
                    break
                i += 1
        self.timer = None
        return False

    def on_style_updated(self, *args):
        #self.loadUpdates()
        print("aa")

    def loadMenus(self):
        self.menu_store.clear()
        for menu in self.editor.getMenus():
            iters = [None]*20
            self.loadMenu(iters, menu)
        menu_tree = self.tree.get_object('menu_tree')
        menu_tree.set_model(self.menu_store)
        for menu in self.menu_store:
            #this might not work for some reason
            try:
                menu_tree.expand_to_path(menu.path)
            except:
                pass
        menu_tree.get_selection().select_path((0,))
        self.on_menu_tree_cursor_changed(menu_tree)

    def loadMenu(self, iters, parent, depth=0):
        if depth == 0:
            icon = getIcon(parent)
            iters[depth] = self.menu_store.append(None, (icon, cgi.escape(parent.get_name().decode('utf-8')), parent))
        depth += 1
        for menu, show in self.editor.getMenus(parent):
            if show:
                name = cgi.escape(menu.get_name().decode('utf-8'))
            else:
                name = '<small><i>' + cgi.escape(menu.get_name().decode('utf-8')) + '</i></small>'
            icon = getIcon(menu)
            iters[depth] = self.menu_store.append(iters[depth-1], (icon, name, menu))
            self.loadMenu(iters, menu, depth)
        depth -= 1

    def loadItems(self, menu, menu_path):
        self.item_store.clear()
        for item, show in self.editor.getItems(menu):
            menu_icon = None
            if item.get_type() == ukuimenu.TYPE_SEPARATOR:
                name = '---'
                icon = None
            elif item.get_type() == ukuimenu.TYPE_ENTRY:
                if show:
                    name = cgi.escape(item.get_display_name().decode('utf-8'))
                else:
                    name = '<small><i>' + cgi.escape(item.get_display_name().decode('utf-8')) + '</i></small>'
                icon = getIcon(item)
            else:
                if show:
                    name = cgi.escape(item.get_name().decode('utf-8'))
                else:
                    name = '<small><i>' + cgi.escape(item.get_name().decode('utf-8')) + '</i></small>'
                icon = getIcon(item)
            self.item_store.append((show, icon, name, item))

    def waitForNewItemProcess(self, process, parent_id, file_path):
        if process.poll() is not None:
            if os.path.isfile(file_path):
                self.editor.insertExternalItem(os.path.split(file_path)[1], parent_id)
            return False
        return True

    def waitForNewMenuProcess(self, process, parent_id, file_path):
        if process.poll() is not None:
            #hack for broken ukui-desktop-item-edit
            broken_path = os.path.join(os.path.split(file_path)[0], '.directory')
            if os.path.isfile(broken_path):
                os.rename(broken_path, file_path)
            if os.path.isfile(file_path):
                self.editor.insertExternalMenu(os.path.split(file_path)[1], parent_id)
            return False
        return True

    #this callback keeps you from editing the same item twice
    def waitForEditProcess(self, process, file_path):
        if process.poll() is not None:
            self.edit_pool.remove(file_path)
            return False
        return True

    def findMenu(self, menus, path, iter, menu_id):
        if not menus[path][2].get_desktop_file_path():
            if menu_id == menus[path][2].get_menu_id():
                menu_tree = self.tree.get_object('menu_tree')
                menu_tree.expand_to_path(path)
                menu_tree.get_selection().select_path(path)
                return True
            return False
        if os.path.split(menus[path][2].get_desktop_file_path())[1] == menu_id:
            menu_tree = self.tree.get_object('menu_tree')
            menu_tree.expand_to_path(path)
            menu_tree.get_selection().select_path(path)
            return True

    def setupMenuTree(self):
        self.menu_store = Gtk.TreeStore(GdkPixbuf.Pixbuf, str, object)
        menus = self.tree.get_object('menu_tree')
        column = Gtk.TreeViewColumn(_('Name'))
        column.set_spacing(4)
        cell = Gtk.CellRendererPixbuf()
        column.pack_start(cell, False)
        column.set_attributes(cell, pixbuf=0)
        cell = Gtk.CellRendererText()
        cell.set_fixed_size(-1, 25)
        column.pack_start(cell, True)
        column.set_attributes(cell, markup=1)
        column.set_sizing(Gtk.TreeViewColumnSizing.FIXED)
        menus.append_column(column)
        menus.enable_model_drag_source(Gdk.ModifierType.BUTTON1_MASK, self.dnd_menus, Gdk.DragAction.COPY)
        menus.enable_model_drag_dest(self.dnd_both, Gdk.DragAction.PRIVATE)

    def setupItemTree(self):
        items = self.tree.get_object('item_tree')
        column = Gtk.TreeViewColumn(_('Show'))
        cell = Gtk.CellRendererToggle()
        cell.connect('toggled', self.on_item_tree_show_toggled)
        column.pack_start(cell, True)
        column.set_attributes(cell, active=0)
        #hide toggle for separators
        column.set_cell_data_func(cell, self._cell_data_toggle_func)
        items.append_column(column)
        column = Gtk.TreeViewColumn(_('Item'))
        column.set_spacing(4)
        cell = Gtk.CellRendererPixbuf()
        column.pack_start(cell, False)
        column.set_attributes(cell, pixbuf=1)
        cell = Gtk.CellRendererText()
        cell.set_fixed_size(-1, 25)
        column.pack_start(cell, True)
        column.set_attributes(cell, markup=2)
        items.append_column(column)
        self.item_store = Gtk.ListStore(bool, GdkPixbuf.Pixbuf, str, object)
        items.set_model(self.item_store)
        items.enable_model_drag_source(Gdk.ModifierType.BUTTON1_MASK, self.dnd_items, Gdk.DragAction.COPY)
        items.enable_model_drag_dest(self.dnd_items, Gdk.DragAction.PRIVATE)

    def _cell_data_toggle_func(self, tree_column, renderer, model, treeiter, data=None):
        if model[treeiter][3].get_type() == ukuimenu.TYPE_SEPARATOR:
            renderer.set_property('visible', False)
        else:
            renderer.set_property('visible', True)

    def on_item_tree_show_toggled(self, cell, path):
        item = self.item_store[path][3]
        if item.get_type() == ukuimenu.TYPE_SEPARATOR:
            return
        if self.item_store[path][0]:
            state = self.editor.setVisible(item, False)
            if state == None:
                return
        else:
            if self.item_store[path][1] == None:
                msg = _("\n[%s] is a system autostart software, you should not show it in startup menu!") % (item.get_name().decode('utf-8'))
                md = Gtk.MessageDialog(None, 0, Gtk.MessageType.WARNING, Gtk.ButtonsType.OK, msg)
                md.run()
                md.destroy()
                return
            state = self.editor.setVisible(item, True)
            if state == None:
                return
        self.item_store[path][0] = not self.item_store[path][0]

    def on_move_up_button_clicked(self, button):
        item_tree = self.tree.get_object('item_tree')
        items, iter = item_tree.get_selection().get_selected()
        if not iter:
            return
        path = items.get_path(iter)
        #at top, can't move up
        if path.get_indices()[0] == 0:
            return
        item = items[path][3]
        before = items[(path[0] - 1,)][3]
        if item.get_type() == ukuimenu.TYPE_ENTRY:
            self.editor.moveItem(item, item.get_parent(), before=before)
        elif item.get_type() == ukuimenu.TYPE_DIRECTORY:
            self.editor.moveMenu(item, item.get_parent(), before=before)
        elif item.get_type() == ukuimenu.TYPE_SEPARATOR:
            self.editor.moveSeparator(item, item.get_parent(), before=before)

    def on_move_down_button_clicked(self, button):
        item_tree = self.tree.get_object('item_tree')
        items, iter = item_tree.get_selection().get_selected()
        if not iter:
            return
        path = items.get_path(iter)
        #at bottom, can't move down
        if path.get_indices()[0] == (len(items) - 1):
            return
        item = items[path][3]
        after = items[path][3]
        if item.get_type() == ukuimenu.TYPE_ENTRY:
            self.editor.moveItem(item, item.get_parent(), after=after)
        elif item.get_type() == ukuimenu.TYPE_DIRECTORY:
            self.editor.moveMenu(item, item.get_parent(), after=after)
        elif item.get_type() == ukuimenu.TYPE_SEPARATOR:
            self.editor.moveSeparator(item, item.get_parent(), after=after)

    def on_delete_button_clicked(self, button):
        self.on_edit_delete_activate(None)

    def on_properties_button_clicked(self, button):
        self.on_edit_properties_activate(None)

    def on_new_separator_button_clicked(self, button):
        item_tree = self.tree.get_object('item_tree')
        items, iter = item_tree.get_selection().get_selected()
        if not iter:
            return
        else:
            after = items[iter][3]
            menu_tree = self.tree.get_object('menu_tree')
            menus, iter = menu_tree.get_selection().get_selected()
            parent = menus[iter][2]
            self.editor.createSeparator(parent, after=after)

    def on_new_item_button_clicked(self, button):
        menu_tree = self.tree.get_object('menu_tree')
        menus, iter = menu_tree.get_selection().get_selected()
        if not iter:
            parent = menus[(0,)][2]
            menu_tree.expand_to_path((0,))
            menu_tree.get_selection().select_path((0,))
        else:
            parent = menus[iter][2]
        file_path = os.path.join(getUserItemPath(), getUniqueFileId('mozo-made', '.desktop'))
        process = subprocess.Popen(['ukui-desktop-item-edit', file_path], env=os.environ)
        GLib.timeout_add(100, self.waitForNewItemProcess, process, parent.menu_id, file_path)

    def on_new_menu_button_clicked(self, button):
        menu_tree = self.tree.get_object('menu_tree')
        menus, iter = menu_tree.get_selection().get_selected()
        if not iter:
            parent = menus[(0,)][2]
            menu_tree.expand_to_path((0,))
            menu_tree.get_selection().select_path((0,))
        else:
            parent = menus[iter][2]
        file_path = os.path.join(getUserDirectoryPath(), getUniqueFileId('mozo-made', '.directory'))
        process = subprocess.Popen(['ukui-desktop-item-edit', file_path], env=os.environ)
        GLib.timeout_add(100, self.waitForNewMenuProcess, process, parent.menu_id, file_path)

    def on_item_tree_popup_menu(self, item_tree, event=None):
        model, iter = item_tree.get_selection().get_selected()
        if event:
            #don't show if it's not the right mouse button
            if event.button != 3:
                return
            button = event.button
            event_time = event.time
            info = item_tree.get_path_at_pos(int(event.x), int(event.y))
            if info is not None:
                path, col, cellx, celly = info
                item_tree.grab_focus()
                item_tree.set_cursor(path, col, 0)
        else:
            path = model.get_path(iter)
            button = 0
            event_time = 0
            item_tree.grab_focus()
            item_tree.set_cursor(path, item_tree.get_columns()[0], 0)
        popup = self.tree.get_object('edit_menu')
        popup.popup(None, None, None, None, button, event_time)
        #without this shift-f10 won't work
        return True

    def on_item_tree_cursor_changed(self, treeview):
        items, iter = treeview.get_selection().get_selected()
        if iter is None:
            return
        item = items[iter][3]
        self.tree.get_object('edit_delete').set_sensitive(True)
        self.tree.get_object('new_separator_button').set_sensitive(True)
        self.tree.get_object('delete_button').set_sensitive(True)

        can_revert = self.editor.canRevert(item)
        self.tree.get_object('edit_revert_to_original').set_sensitive(can_revert)

        can_edit = not item.get_type() == ukuimenu.TYPE_SEPARATOR
        self.tree.get_object('edit_properties').set_sensitive(can_edit)
        self.tree.get_object('properties_button').set_sensitive(can_edit)

        index = items.get_path(iter).get_indices()[0]
        can_go_up = index > 0
        can_go_down = index < len(items) - 1
        self.tree.get_object('move_up_button').set_sensitive(can_go_up)
        self.tree.get_object('move_down_button').set_sensitive(can_go_down)

    def on_item_tree_drag_data_get(self, treeview, context, selection, target_id, etime):
        items, iter = treeview.get_selection().get_selected()
        self.drag_data = items[iter][3]

    def on_item_tree_drag_data_received(self, treeview, context, x, y, selection, info, etime):
        items = treeview.get_model()
        types_before = (Gtk.TreeViewDropPosition.BEFORE, Gtk.TreeViewDropPosition.INTO_OR_BEFORE)
        types_into = (Gtk.TreeViewDropPosition.INTO_OR_BEFORE, Gtk.TreeViewDropPosition.INTO_OR_AFTER)
        types_after = (Gtk.TreeViewDropPosition.AFTER, Gtk.TreeViewDropPosition.INTO_OR_AFTER)
        if str(selection.get_target()) == 'MOZO_ITEM_ROW':
            drop_info = treeview.get_dest_row_at_pos(x, y)
            before = None
            after = None
            if self.drag_data is None:
                return False
            item = self.drag_data
            # by default we assume, that the items stays in the same menu
            destination = item.get_parent()
            if drop_info:
                path, position = drop_info
                target = items[path][3]
                # move the item to the directory, if the item was dropped into it
                if (target.get_type() == ukuimenu.TYPE_DIRECTORY) and (position in types_into):
                    # append the selected item to the choosen menu
                    destination = target
                elif position in types_before:
                    before = target
                elif position in types_after:
                    after = target
                else:
                    # this does not happen
                    pass
            else:
                path = (len(items) - 1,)
                after = items[path][3]
            if item.get_type() == ukuimenu.TYPE_ENTRY:
                self.editor.moveItem(item, destination, before, after)
            elif item.get_type() == ukuimenu.TYPE_DIRECTORY:
                if not self.editor.moveMenu(item, destination, before, after):
                    self.loadUpdates()
            elif item.get_type() == ukuimenu.TYPE_SEPARATOR:
                self.editor.moveSeparator(item, destination, before, after)
            context.finish(True, True, etime)
        elif str(selection.get_target()) == 'text/plain':
            if selection.data is None:
                return False
            menus, iter = self.tree.get_object('menu_tree').get_selection().get_selected()
            parent = menus[iter][2]
            drop_info = treeview.get_dest_row_at_pos(x, y)
            before = None
            after = None
            if drop_info:
                path, position = drop_info
                if position in types_before:
                    before = items[path][3]
                else:
                    after = items[path][3]
            else:
                path = (len(items) - 1,)
                after = items[path][3]
            file_path = urllib.unquote(selection.data).strip()
            if not file_path.startswith('file:'):
                return
            myfile = Gio.File(uri=file_path)
            file_info = myfile.query_info(Gio.FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE)
            content_type = file_info.get_content_type()
            if content_type == 'application/x-desktop':
                input_stream = myfile.read()
                keyfile = GLib.KeyFile()
                keyfile.load_from_data(input_stream.read())
                self.editor.createItem(parent, before, after, KeyFile=keyfile)
            elif content_type in ('application/x-shellscript', 'application/x-executable'):
                self.editor.createItem(parent, before, after,
                                       Name=os.path.split(file_path)[1].strip(),
                                       Exec=file_path.replace('file://', '').strip(),
                                       Terminal=False)
        self.drag_data = None

    def on_item_tree_key_press_event(self, item_tree, event):
        if event.keyval == Gdk.KEY_Delete:
            self.on_edit_delete_activate(item_tree)

    def on_item_tree_row_activated(self, treeview, path, column):
        self.on_edit_properties_activate(None)

    def on_menu_tree_cursor_changed(self, treeview):
        menus, iter = treeview.get_selection().get_selected()
        if iter is None:
            return
        menu_path = menus.get_path(iter)
        item_tree = self.tree.get_object('item_tree')
        item_tree.get_selection().unselect_all()
        self.loadItems(self.menu_store[menu_path][2], menu_path)
        self.tree.get_object('edit_delete').set_sensitive(False)
        self.tree.get_object('edit_revert_to_original').set_sensitive(False)
        self.tree.get_object('edit_properties').set_sensitive(False)
        self.tree.get_object('move_up_button').set_sensitive(False)
        self.tree.get_object('move_down_button').set_sensitive(False)
        self.tree.get_object('new_separator_button').set_sensitive(False)

    def on_revert_button_clicked(self, button):
        dialog = self.tree.get_object('revertdialog')
        dialog.set_transient_for(self.tree.get_object('mainwindow'))
        dialog.show_all()
        if dialog.run() == Gtk.ResponseType.YES:
            self.editor.revert()
            dialog.hide()

    def on_help_button_clicked(self, *args):
        Gtk.show_uri(Gdk.Screen.get_default(), "help:ukui-user-guide/menu-editor", Gtk.get_current_event_time())

    def on_edit_delete_activate(self, menu):
        item_tree = self.tree.get_object('item_tree')
        items, iter = item_tree.get_selection().get_selected()
        if not iter:
            return
        item = items[iter][3]
        if item.get_type() == ukuimenu.TYPE_ENTRY:
            self.editor.deleteItem(item)
        elif item.get_type() == ukuimenu.TYPE_DIRECTORY:
            self.editor.deleteMenu(item)
        elif item.get_type() == ukuimenu.TYPE_SEPARATOR:
            self.editor.deleteSeparator(item)

    def on_edit_revert_to_original_activate(self, menu):
        item_tree = self.tree.get_object('item_tree')
        items, iter = item_tree.get_selection().get_selected()
        if not iter:
            return
        item = items[iter][3]
        if item.get_type() == ukuimenu.TYPE_ENTRY:
            self.editor.revertItem(item)
        elif item.get_type() == ukuimenu.TYPE_DIRECTORY:
            self.editor.revertMenu(item)

    def on_edit_properties_activate(self, menu):
        item_tree = self.tree.get_object('item_tree')
        items, iter = item_tree.get_selection().get_selected()
        if not iter:
            return
        item = items[iter][3]
        if item.get_type() not in (ukuimenu.TYPE_ENTRY, ukuimenu.TYPE_DIRECTORY):
            return

        if item.get_type() == ukuimenu.TYPE_ENTRY:
            file_path = os.path.join(getUserItemPath(), item.get_desktop_file_id().decode('utf-8'))
            file_type = 'Item'
        elif item.get_type() == ukuimenu.TYPE_DIRECTORY:
            file_path = os.path.join(getUserDirectoryPath(), os.path.split(item.get_desktop_file_path().decode('utf-8'))[1])
            file_type = 'Menu'

        if not os.path.isfile(file_path):
            shutil.copy(item.get_desktop_file_path().decode('utf-8'), file_path)
            self.editor._MenuEditor__addUndo([(file_type, os.path.split(file_path)[1]),])
        else:
            self.editor._MenuEditor__addUndo([item,])
        if file_path not in self.edit_pool:
            self.edit_pool.append(file_path)
            process = subprocess.Popen(['ukui-desktop-item-edit', file_path], env=os.environ)
            GLib.timeout_add(100, self.waitForEditProcess, process, file_path)

    def on_mainwindow_undo(self, accelgroup, window, keyval, modifier):
        self.editor.undo()

    def on_mainwindow_redo(self, accelgroup, window, keyval, modifier):
        self.editor.redo()

    def on_menu_tree_drag_data_get(self, treeview, context, selection, target_id, etime):
        menus, iter = treeview.get_selection().get_selected()
        self.drag_data = menus[iter][2]

    def on_menu_tree_drag_data_received(self, treeview, context, x, y, selection, info, etime):
        menus = treeview.get_model()
        drop_info = treeview.get_dest_row_at_pos(x, y)
        if drop_info:
            path, position = drop_info
            types = (Gtk.TreeViewDropPosition.INTO_OR_BEFORE, Gtk.TreeViewDropPosition.INTO_OR_AFTER)
            if position not in types:
                context.finish(False, False, etime)
                return False
            if str(selection.get_target()) in ('MOZO_ITEM_ROW', 'MOZO_MENU_ROW'):
                if self.drag_data is None:
                    return False
                item = self.drag_data
                new_parent = menus[path][2]
                if item.get_type() == ukuimenu.TYPE_ENTRY:
                    self.editor.copyItem(item, new_parent)
                elif item.get_type() == ukuimenu.TYPE_DIRECTORY:
                    if not self.editor.moveMenu(item, new_parent):
                        self.loadUpdates()
                elif item.get_type() == ukuimenu.TYPE_SEPARATOR:
                    self.editor.moveSeparator(item, new_parent)
                else:
                    context.finish(False, False, etime)
                context.finish(True, True, etime)
            
            self.drag_data = None

