#ifndef REALITYDATA_H_
#define REALITYDATA_H_

#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtCore/QString>
#include "SharedMemory.h"

class RealityData : public QObject
{
	public:
		RealityData();
		virtual ~RealityData();
		
		void update();
		bool isEnabled();
	protected:
		void setData(QByteArray data);
		
		bool _isEnabled;
		SharedMemory2 *smMem;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(RealityData)
};

#endif