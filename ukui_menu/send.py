#!/usr/bin/python3
# -*- coding: UTF-8 -*-

import os
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email.utils import formataddr
from email import encoders

syslog_file = '/var/log/syslog'
kernlog_file = '/var/log/kern.log'
dpkglog_file = '/var/log/dpkg.log'

class Send:
    _attachments = []
    def __init__(self):
        self.receivers = ['jianfengli@ubuntukylin.com']

    def add_attachment(self, filename):
        att = MIMEBase('application', 'octet-stream')
        att.set_payload(open(filename, 'rb').read())
        att.add_header('Content-Disposition', 'attachment', filename = ('utf-8', '', filename))
        encoders.encode_base64(att)
        self._attachments.append(att)

    def send(self, fb_type, body, sender, attach_file, syslog, kernel_log, dpkg_log):
        print("type: %s\n body: %s\n email %s\n" % (fb_type, body, sender))
        self.sender = sender
        self.message = MIMEMultipart('altermative')
        contents = MIMEText(body, 'plain', 'utf-8')
        self.message['From'] = formataddr(["youker", sender])
        self.message['To'] = formataddr(["ubuntukylin", self.receivers[0]])
        self.message['Subject'] = fb_type
        if (os.path.isfile(attach_file)):
            self.add_attachment(attach_file)
        if (syslog and os.path.isfile(syslog_file) and os.access(syslog_file, os.R_OK)):
            self.add_attachment(syslog_file)
        if (kernel_log and os.path.isfile(kernlog_file) and os.access(kernlog_file, os.R_OK)):
            self.add_attachment(kernlog_file)
        if (dpkg_log and os.path.isfile(dpkglog_file) and os.access(dpkglog_file, os.R_OK)):
            self.add_attachment(dpkglog_file)

        for att in self._attachments:
            self.message.attach(att)
        self.message.attach(contents)
        try:
            smtpObj = smtplib.SMTP('localhost')
            smtpObj.sendmail(self.sender, self.receivers, self.message.as_string())
            smtpObj.quit()
            print("发送成功")
        except smtplib.SMTPException:
            print("Error: 无法发送邮件")

if __name__ == '__main__':
    send = Send()
    print("send")
