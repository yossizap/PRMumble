/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "Player.h"
#include "Channel.h"

QHash<short, Player *> Player::c_qmPlayers;
QReadWriteLock Player::c_qrwlPlayers;

Player::Player(QObject *p) : QObject(p) {
  sState = Player::Connected;
  sId = 0;
  iId = -1;
  bMute = bDeaf = false;
  bSelfMute = bSelfDeaf = false;
  bTalking = bAltSpeak = false;
  bLocalMute = false;
  bSuppressed = false;
  cChannel = 0;
}

Player *Player::get(short sId) {
	QReadLocker lock(&c_qrwlPlayers);
	Player *p = c_qmPlayers.value(sId);
	return p;
}

Player *Player::add(short sId, QObject *po) {
	QWriteLocker lock(&c_qrwlPlayers);

	Player *p = new Player(po);
	p->sId = sId;
	c_qmPlayers[sId] = p;
	return p;
}

Player *Player::match(const Player *other, bool matchname) {
	QReadLocker lock(&c_qrwlPlayers);

	Player *p;
	foreach(p, c_qmPlayers) {
		if (p == other)
			continue;
		if ((p->iId >= 0) && (p->iId == other->iId))
			return p;
		if (matchname && (p->qsName == other->qsName))
			return p;
	}
	return NULL;
}

void Player::remove(short sId) {
	QWriteLocker lock(&c_qrwlPlayers);
	Player *p = c_qmPlayers.take(sId);
	if (p && p->cChannel)
		p->cChannel->removePlayer(p);
}

void Player::remove(Player *p) {
	remove(p->sId);
}

void Player::setTalking(bool talking, bool altspeech) {
	if ((bTalking == talking) && (bAltSpeak == altspeech))
		return;
	bTalking = talking;
	bAltSpeak = altspeech;
	emit talkingChanged(bTalking);
}

void Player::setMute(bool mute) {
	if (bMute == mute)
		return;
	bMute = mute;
	if (! bMute && bDeaf)
		bDeaf = false;
	emit muteDeafChanged();
}

void Player::setLocalMute(bool mute) {
	if (bLocalMute == mute)
		return;
	bLocalMute = mute;
	emit muteDeafChanged();
}

void Player::setDeaf(bool deaf) {
	if (bDeaf == deaf)
		return;
	bDeaf = deaf;
	if (bDeaf && ! bMute)
		bMute = true;
	emit muteDeafChanged();
}

void Player::setSelfMuteDeaf(bool mute, bool deaf) {
	bSelfDeaf = deaf;
	bSelfMute = mute;
	emit muteDeafChanged();
}
