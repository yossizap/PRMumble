#include "mumble_pch.hpp"

#include "RealityData.h"

#include "ClientUser.h"
#include "Channel.h"
#include "Global.h"
#include "Settings.h"

RealityData::RealityData()
{
	_isEnabled = false;
	smMem = NULL;
	
	smMem = new SharedMemory2(this, 32768, QLatin1String("PRMumbleSharedData"), true);
	if (!smMem || !smMem->data()) {
		qWarning() << "RealityData: Failed to create shared memory";
		delete smMem;
		smMem = NULL;
		return;
	}
	
	_isEnabled = true;
}

RealityData::~RealityData()
{
	try {
		if (smMem)
			delete smMem;
	} catch (...) {
	}
}

void RealityData::update()
{
	if (!_isEnabled || !smMem)
		return;
	
	try {
		ClientUser *self = ClientUser::get(g.uiSession);
		
		if (!self) {
			if (smMem)
				smMem->erase();
			return;
		}
		
		Channel *home = self->cChannel;
		
		QList<ClientUser *> users;
		
		foreach(Channel *c, home->allLinks())
			foreach(User *p, c->qlUsers)
				users << static_cast<ClientUser *>(p);
		
		foreach(ClientUser *cu, ClientUser::getTalking())
			if (!users.contains(cu))
				users << cu;
		
		if (users.count() == 0) {
			if (smMem)
				smMem->erase();
			return;
		}
		
		QStringList talking;
		foreach(ClientUser *cu, users) {
			if (!cu || !cu->cChannel)
				continue;
			
			if (cu != self)
				if (cu->bMute || cu->bDeaf || cu->bSuppress || cu->bSelfMute || cu->bSelfDeaf || cu->bLocalMute || cu->bLocalIgnore)
					continue;
			
			QString identity = cu->qsIdentity;
			QString name = cu->qsName;
			
			if (name.isEmpty())
				continue;
			
			if (!identity.isEmpty()) {
				int nameIndex = identity.indexOf(QLatin1String("\"name\":\""), 0, Qt::CaseInsensitive);
				if (nameIndex >= 0) {
					nameIndex += 8;
					int nameFinishIndex = identity.indexOf(QLatin1String("\""), nameIndex + 1);
					if (nameFinishIndex >= 0 && nameIndex < nameFinishIndex && nameIndex < identity.length() && nameFinishIndex < identity.length()) {
						QString tempName = identity.mid(nameIndex, nameFinishIndex - nameIndex);
						if (!tempName.isEmpty())
							name = tempName;
					}
				}
			}
			
			talking << QString::fromUtf8("%1\x01%2\x01%3\x01%4\x01%5\x01%6\x01%7\x01%8")
				.arg(name)
				.arg(cu->cChannel->qsName)
				.arg((int)cu->tsState)
				.arg((cu->bMute || cu->bLocalMute || cu->bSelfMute) ? 1 : 0)
				.arg((cu->bDeaf || cu->bSelfDeaf) ? 1 : 0)
				.arg((double)cu->fPosition[0], 0, 'f', 0)
				.arg((double)cu->fPosition[1], 0, 'f', 0)
				.arg((double)cu->fPosition[2], 0, 'f', 0);
		}
		
		if (talking.count() > 0)
			setData(talking.join(QLatin1String("\x02")).toUtf8());
		else if (smMem)
			smMem->erase();
	} catch (...) {
		if (smMem)
			smMem->erase();
	}
}

void RealityData::setData(QByteArray data)
{
	try {
		if (smMem) {
			smMem->erase();
			smMem->setData((unsigned char*)data.data(), data.size() >= 32768 ? 32767 : data.size());
		}
	} catch (...) {
	}
}

bool RealityData::isEnabled()
{
	return _isEnabled;
}
