// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "mumble_pch.hpp"

#include "About.h"

#include "Global.h"
#include "MainWindow.h"
#include "License.h"

AboutDialog::AboutDialog(QWidget *p) : QDialog(p) {
	setWindowTitle(tr("About Mumble"));

	QTabWidget *qtwTab = new QTabWidget(this);
	QVBoxLayout *vblMain = new QVBoxLayout(this);

	QTextEdit *qteLicense = new QTextEdit(qtwTab);
	qteLicense->setReadOnly(true);
	qteLicense->setPlainText(License::license());

	QTextEdit *qteAuthors = new QTextEdit(qtwTab);
	qteAuthors->setReadOnly(true);
	qteAuthors->setPlainText(License::authors());

	QTextBrowser *qtb3rdPartyLicense = new QTextBrowser(qtwTab);
	qtb3rdPartyLicense->setReadOnly(true);
	qtb3rdPartyLicense->setOpenExternalLinks(true);

	QList<LicenseInfo> thirdPartyLicenses = License::thirdPartyLicenses();
	foreach(LicenseInfo li, thirdPartyLicenses) {
		qtb3rdPartyLicense->append(QString::fromLatin1("<h3>%1 (<a href=\"%2\">%2</a>)</h3><pre>%3</pre>")
				.arg(Qt::escape(li.name))
				.arg(Qt::escape(li.url))
				.arg(Qt::escape(li.license)));
	}

	qtb3rdPartyLicense->moveCursor(QTextCursor::Start);

	QWidget *about = new QWidget(qtwTab);

	QLabel *icon = new QLabel(about);
	icon->setPixmap(g.mw->qiIcon.pixmap(g.mw->qiIcon.actualSize(QSize(128, 128))));

	QLabel *text = new QLabel(about);
	text->setOpenExternalLinks(true);
	text->setText(tr(
	                  "<h3>Project Reality Mumble %1</h3>"
	                  "<p>Copyright (C) %5 Project Reality<br /><tt><a href=\"%3\">%3</a></tt></p>"
	                  "<p>Copyright %4 Thorvald Natvig<br />slicer@users.sourceforge.net</p>"
	                  "<p><b>A voice-chat utility for gamers, customized for Project Reality</b></p>"
	                  "<p><tt><a href=\"%3\">%3</a></tt></p>"
	                  "<p><tt><a href=\"%2\">%2</a></tt></p>"
	              ).arg(QLatin1String(MUMBLE_RELEASE))
				  .arg(QLatin1String("http://mumble.sourceforge.net/"))
				  .arg(QLatin1String("http://www.realitymod.com/"))
				  .arg(QLatin1String("2005-2012"))
				  .arg(QLatin1String("2013")));
	QHBoxLayout *qhbl=new QHBoxLayout(about);
	qhbl->addWidget(icon);
	qhbl->addWidget(text);

	qtwTab->addTab(about, tr("&About PR Mumble"));
	qtwTab->addTab(qteLicense, tr("&License"));
	qtwTab->addTab(qtb3rdPartyLicense, tr("&Third-Party Licenses"));

	QPushButton *okButton = new QPushButton(tr("OK"), this);
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

	vblMain->addWidget(qtwTab);
	vblMain->addWidget(okButton);
}
