// info_dialog.h
//
// Display System Information for Rivendell
//
//   (C) Copyright 2002-2018 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
#ifndef INFO_DIALOG_H
#define INFO_DIALOG_H

#include <rddialog.h>

//
// Globals
//
extern const unsigned char xpm_infobanner1[];
extern const unsigned char xpm_infobanner2[];

class InfoDialog : public RDDialog
{
 Q_OBJECT
 public:
  InfoDialog(QWidget *parent=0);
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void viewCreditsData();
  void viewLicenseData();
  void closeData();
};


#endif

