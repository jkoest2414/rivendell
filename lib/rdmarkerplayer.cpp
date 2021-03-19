// rdmarkerplayer.cpp
//
// Audio player for RDMarkerDialog
//
//   (C) Copyright 2021 Fred Gleason <fredg@paravelsystems.com>
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

#include "rdmixer.h"
#include "rdmarkerplayer.h"

RDMarkerPlayer::RDMarkerPlayer(int card,int port,QWidget *parent)
  : RDWidget(parent)
{
  d_cards.push_back(card);
  d_port=port;
  d_cae_stream=-1;
  d_cae_handle=-1;
  d_is_playing=false;

  //
  // CAE
  //
  rda->cae()->enableMetering(&d_cards);
  connect(rda->cae(),SIGNAL(playing(int)),this,SLOT(caePlayedData(int)));
  connect(rda->cae(),SIGNAL(playStopped(int)),this,SLOT(caePausedData(int)));
  connect(rda->cae(),SIGNAL(playPositionChanged(int,unsigned)),
	  this,SLOT(caePositionData(int,unsigned)));

  //
  // Time Counters
  //
  d_overall_label=new QLabel(tr("Position"),this);
  d_overall_label->setFont(subLabelFont());
  d_overall_label->setAlignment(Qt::AlignHCenter);
  d_overall_label->
    setPalette(QPalette(palette().color(QPalette::Background),
			QColor(RDMARKERPLAYER_HIGHLIGHT_COLOR)));
  d_overall_edit=new QLineEdit(this);
  d_overall_edit->setAcceptDrops(false);
  d_overall_edit->setReadOnly(true);

  d_region_edit_label=new QLabel("Region",this);
  d_region_edit_label->setFont(subLabelFont());
  d_region_edit_label->setAlignment(Qt::AlignHCenter);
  d_region_edit_label->
    setPalette(QPalette(palette().color(QPalette::Background),QColor(RDMARKERPLAYER_HIGHLIGHT_COLOR)));
  d_region_edit=new QLineEdit(this);
  d_region_edit->setAcceptDrops(false);
  d_region_edit->setReadOnly(true);

  d_size_label=new QLabel(tr("Length"),this);
  d_size_label->setFont(subLabelFont());
  d_size_label->setAlignment(Qt::AlignHCenter);
  d_size_label->
    setPalette(QPalette(palette().color(QPalette::Background),QColor(RDMARKERPLAYER_HIGHLIGHT_COLOR)));
  d_size_edit=new QLineEdit(this);
  d_size_edit->setAcceptDrops(false);
  d_size_edit->setReadOnly(true);

  //
  // Transport Buttons
  //
  d_play_cursor_button=
    new RDTransportButton(RDTransportButton::PlayBetween,this);
  d_play_cursor_button->setFocusPolicy(Qt::NoFocus);
  d_play_cursor_button->setEnabled((d_cards.first()>=0)&&(d_port>=0));
  connect(d_play_cursor_button,SIGNAL(clicked()),
  	  this,SLOT(playCursorData()));

  d_play_start_button=
    new RDTransportButton(RDTransportButton::Play,this);
  d_play_start_button->setFocusPolicy(Qt::NoFocus);
  d_play_start_button->setEnabled((d_cards.first()>=0)&&(d_port>=0));
  connect(d_play_start_button,SIGNAL(clicked()),
  	  this,SLOT(playStartData()));

  d_pause_button=new RDTransportButton(RDTransportButton::Pause,this);
  d_pause_button->setFocusPolicy(Qt::NoFocus);
  d_pause_button->setOnColor(QColor(Qt::red));
  d_pause_button->setEnabled((d_cards.first()>=0)&&(d_port>=0));
  connect(d_pause_button,SIGNAL(clicked()),this,SLOT(pauseData()));

  d_stop_button=new RDTransportButton(RDTransportButton::Stop,this);
  d_stop_button->setFocusPolicy(Qt::NoFocus);
  d_stop_button->on();
  d_stop_button->setOnColor(QColor(Qt::red));
  d_stop_button->setEnabled((d_cards.first()>=0)&&(d_port>=0));
  connect(d_stop_button,SIGNAL(clicked()),this,SLOT(stopData()));

  d_loop_button=new RDTransportButton(RDTransportButton::Loop,this);
  d_loop_button->off();
  d_loop_button->setEnabled((d_cards.first()>=0)&&(d_port>=0));
  connect(d_loop_button,SIGNAL(clicked()),this,SLOT(loopData()));

  //
  // The Audio Meter
  //
  d_meter=new RDStereoMeter(this);
  d_meter->setSegmentSize(5);
  d_meter->setMode(RDSegMeter::Peak);
  d_meter_timer=new QTimer(this);
  connect(d_meter_timer,SIGNAL(timeout()),this,SLOT(meterData()));
}


RDMarkerPlayer::~RDMarkerPlayer()
{
}


QSize RDMarkerPlayer::sizeHint() const
{
  return QSize(717,92);
}


QSizePolicy RDMarkerPlayer::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


bool RDMarkerPlayer::setCut(unsigned cartnum,int cutnum)
{
  clearCut();

  if(!rda->cae()->loadPlay(d_cards.first(),RDCut::cutName(cartnum,cutnum),
			   &d_cae_stream,&d_cae_handle)) {
    return false;
  }
  RDSetMixerOutputPort(rda->cae(),d_cards.first(),d_cae_stream,d_port);

  return true;
}


void RDMarkerPlayer::clearCut()
{
  if(d_cae_handle>=0) {
    rda->cae()->stopPlay(d_cae_handle);
    rda->cae()->unloadPlay(d_cae_handle);
    d_cae_stream=-1;
    d_cae_handle=-1;
    d_is_playing=false;
  }
  for(int i=0;i<RDMarkerHandle::LastRole;i++) {
    d_pointers[i]=-1;
  }
}


void RDMarkerPlayer::setPlayPosition(int msec)
{
}


void RDMarkerPlayer::setPointerValue(RDMarkerHandle::PointerRole role,int ptr)
{
  d_pointers[role]=ptr;
}


void RDMarkerPlayer::playCursorData()
{
}


void RDMarkerPlayer::playStartData()
{
  if(d_cae_handle>=0) {
    if(d_is_playing) {
      rda->cae()->stopPlay(d_cae_handle);
    }
    rda->cae()->positionPlay(d_cae_handle,d_pointers[RDMarkerHandle::CutStart]);
    rda->cae()->play(d_cae_handle,
		     d_pointers[RDMarkerHandle::CutEnd]-
		     d_pointers[RDMarkerHandle::CutStart],100000,false);
    rda->cae()->setPlayPortActive(d_cards.first(),d_port,d_cae_stream);
    // FIXME: Implement variable gain here!
    rda->cae()->setOutputVolume(d_cards.first(),d_cae_stream,d_port,0);
    //    rda->cae()->
    //      setOutputVolume(d_cards.first(),d_cae_stream,d_port,0+edit_gain_control->value());
    d_meter_timer->start(RD_METER_UPDATE_INTERVAL);
  }
}


void RDMarkerPlayer::pauseData()
{
}


void RDMarkerPlayer::stopData()
{
  if(d_cae_handle>=0) {
    if(d_is_playing) {
      rda->cae()->stopPlay(d_cae_handle);
    }
  }
}


void RDMarkerPlayer::loopData()
{
}


void RDMarkerPlayer::meterData()
{
  short lvls[2];

  rda->cae()->outputMeterUpdate(d_cards.first(),d_port,lvls);
  d_meter->setLeftPeakBar(lvls[0]);
  d_meter->setRightPeakBar(lvls[1]);
}


void RDMarkerPlayer::caePlayedData(int handle)
{
  if(handle==d_cae_handle) {
    if(!d_is_playing) {
      d_play_start_button->setState(RDTransportButton::On);
      d_pause_button->setState(RDTransportButton::Off);
      d_stop_button->setState(RDTransportButton::Off);
      d_is_playing=true;
    }
  }
}


void RDMarkerPlayer::caePausedData(int handle)
{
  if(handle==d_cae_handle) {
    if(d_is_playing) {
      if(d_meter_timer->isActive()) {
	d_meter_timer->stop();
	d_meter->setLeftPeakBar(-10000);
	d_meter->setRightPeakBar(-10000);
      }
      d_play_start_button->setState(RDTransportButton::Off);
      d_pause_button->setState(RDTransportButton::Off);
      d_stop_button->setState(RDTransportButton::On);
      d_is_playing=false;
    }
  }
}


void RDMarkerPlayer::caePositionData(int handle,unsigned msec)
{
  if(handle==d_cae_handle) {
    emit cursorPositionChanged(msec);
  }
}


void RDMarkerPlayer::resizeEvent(QResizeEvent *)
{
  d_overall_label->setGeometry(50,3,70,20);
  d_overall_edit->setGeometry(50,18,70,21);
  d_region_edit_label->setGeometry(148,3,70,20);
  d_region_edit->setGeometry(148,18,70,21);
  d_size_label->setGeometry(246,3,70,20);
  d_size_edit->setGeometry(246,18,70,21);

  d_play_cursor_button->setGeometry(10,42,65,45);
  d_play_start_button->setGeometry(80,42,65,45);
  d_pause_button->setGeometry(150,42,65,45);
  d_stop_button->setGeometry(220,42,65,45);
  d_loop_button->setGeometry(290,42,65,45);

  d_meter->setGeometry(370,15,d_meter->sizeHint().width(),
		       d_meter->sizeHint().height());
}


void RDMarkerPlayer::paintEvent(QPaintEvent *e)
{
  QPainter *p=new QPainter(this);

  //
  // Transport Control Area
  //
  p->setPen(QColor(palette().shadow().color()));
  p->fillRect(1,1,size().width()-2,size().height()-2,
	      QColor(RDMARKERPLAYER_HIGHLIGHT_COLOR));
  p->drawRect(0,0,size().width(),size().height());

  delete p;
}