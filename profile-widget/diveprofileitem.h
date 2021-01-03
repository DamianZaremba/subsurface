// SPDX-License-Identifier: GPL-2.0
#ifndef DIVEPROFILEITEM_H
#define DIVEPROFILEITEM_H

#include <QObject>
#include <QGraphicsPolygonItem>
#include <QModelIndex>

#include "divelineitem.h"

/* This is the Profile Item, it should be used for quite a lot of things
 on the profile view. The usage should be pretty simple:

 DiveProfileItem *profile = new DiveProfileItem( DiveDataModel );
 profile->setVerticalAxis( profileYAxis );
 profile->setHorizontalAxis( timeAxis );
 profile->setModel( DiveDataModel );
 profile->setHorizontalDataColumn( DiveDataModel::TIME );
 profile->setVerticalDataColumn( DiveDataModel::DEPTH );
 scene()->addItem(profile);

 This is a generically item and should be used as a base for others, I think...
*/

class ProfileWidget2;
class DivePlotDataModel;
class DiveTextItem;
class DiveCartesianAxis;
class QAbstractTableModel;
struct plot_data;

class AbstractProfilePolygonItem : public QObject, public QGraphicsPolygonItem {
	Q_OBJECT
	Q_PROPERTY(QPointF pos WRITE setPos READ pos)
	Q_PROPERTY(qreal x WRITE setX READ x)
	Q_PROPERTY(qreal y WRITE setY READ y)
public:
	AbstractProfilePolygonItem(const DivePlotDataModel &model, const DiveCartesianAxis &hAxis, int hColumn, const DiveCartesianAxis &vAxis, int vColumn);
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) = 0;
	virtual void replot();
public
slots:
	void setVisible(bool visible);

protected:
	/* when the model emits a 'datachanged' signal, this method below should be used to check if the
	 * modified data affects this particular item ( for example, when setting the '3m increment'
	 * the data for Ceiling and tissues will be changed, and only those. so, the topLeft will be the CEILING
	 * column and the bottomRight will have the TISSUE_16 column. this method takes the vDataColumn and hDataColumn
	 * into consideration when returning 'true' for "yes, continue the calculation', and 'false' for
	 * 'do not recalculate, we already have the right data.
	 */
	bool shouldCalculateStuff(const QModelIndex &topLeft, const QModelIndex &bottomRight);

	const DiveCartesianAxis &hAxis;
	const DiveCartesianAxis &vAxis;
	const DivePlotDataModel &dataModel;
	int hDataColumn;
	int vDataColumn;
	QList<DiveTextItem *> texts;
};

class DiveProfileItem : public AbstractProfilePolygonItem {
	Q_OBJECT

public:
	DiveProfileItem(const DivePlotDataModel &model, const DiveCartesianAxis &hAxis, int hColumn, const DiveCartesianAxis &vAxis, int vColumn);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
	void replot() override;
	void settingsToggled(bool toggled);
	void plot_depth_sample(struct plot_data *entry, QFlags<Qt::AlignmentFlag> flags, const QColor &color);
	int maxCeiling(int row);

private:
	unsigned int show_reported_ceiling;
	unsigned int reported_ceiling_in_red;
	QColor profileColor;
};

class DiveMeanDepthItem : public AbstractProfilePolygonItem {
	Q_OBJECT
public:
	DiveMeanDepthItem(const DivePlotDataModel &model, const DiveCartesianAxis &hAxis, int hColumn, const DiveCartesianAxis &vAxis, int vColumn);
	void replot() override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
	void createTextItem();
	double lastRunningSum;
	QString visibilityKey;
};

class DiveTemperatureItem : public AbstractProfilePolygonItem {
	Q_OBJECT
public:
	DiveTemperatureItem(const DivePlotDataModel &model, const DiveCartesianAxis &hAxis, int hColumn, const DiveCartesianAxis &vAxis, int vColumn);
	void replot() override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
	void createTextItem(int seconds, int mkelvin);
};

class DiveHeartrateItem : public AbstractProfilePolygonItem {
	Q_OBJECT
public:
	DiveHeartrateItem(const DivePlotDataModel &model, const DiveCartesianAxis &hAxis, int hColumn, const DiveCartesianAxis &vAxis, int vColumn);
	void replot() override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
	void createTextItem(int seconds, int hr);
	QString visibilityKey;
};

class DivePercentageItem : public AbstractProfilePolygonItem {
	Q_OBJECT
public:
	DivePercentageItem(const DivePlotDataModel &model, const DiveCartesianAxis &hAxis, int hColumn, const DiveCartesianAxis &vAxis, int vColumn, int i);
	void replot() override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
	std::vector<QColor> colors;	// Must have same number of elements as the polygon
	QString visibilityKey;
	int tissueIndex;
	QColor ColorScale(double value, int inert);

};

class DiveAmbPressureItem : public AbstractProfilePolygonItem {
	Q_OBJECT
public:
	DiveAmbPressureItem(const DivePlotDataModel &model, const DiveCartesianAxis &hAxis, int hColumn, const DiveCartesianAxis &vAxis, int vColumn);
	void replot() override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
	QString visibilityKey;
};

class DiveGFLineItem : public AbstractProfilePolygonItem {
	Q_OBJECT
public:
	DiveGFLineItem(const DivePlotDataModel &model, const DiveCartesianAxis &hAxis, int hColumn, const DiveCartesianAxis &vAxis, int vColumn);
	void replot() override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
	QString visibilityKey;
};

class DiveGasPressureItem : public AbstractProfilePolygonItem {
	Q_OBJECT

public:
	using AbstractProfilePolygonItem::AbstractProfilePolygonItem;
	void replot() override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
	void plotPressureValue(int mbar, int sec, QFlags<Qt::AlignmentFlag> align, double offset);
	void plotGasValue(int mbar, int sec, struct gasmix gasmix, QFlags<Qt::AlignmentFlag> align, double offset);
	struct Entry {
		QPointF pos;
		QColor col;
	};
	std::vector<std::vector<Entry>> polygons;
};

class DiveCalculatedCeiling : public AbstractProfilePolygonItem {
	Q_OBJECT

public:
	DiveCalculatedCeiling(const DivePlotDataModel &model, const DiveCartesianAxis &hAxis, int hColumn,
			      const DiveCartesianAxis &vAxis, int vColumn, ProfileWidget2 *profileWidget);
	void replot() override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
	ProfileWidget2 *profileWidget;
};

class DiveReportedCeiling : public AbstractProfilePolygonItem {
	Q_OBJECT

public:
	DiveReportedCeiling(const DivePlotDataModel &model, const DiveCartesianAxis &hAxis, int hColumn, const DiveCartesianAxis &vAxis, int vColumn);
	void replot() override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
};

class DiveCalculatedTissue : public DiveCalculatedCeiling {
	Q_OBJECT
public:
	DiveCalculatedTissue(const DivePlotDataModel &model, const DiveCartesianAxis &hAxis, int hColumn,
			     const DiveCartesianAxis &vAxis, int vColumn, ProfileWidget2 *profileWidget);
	void setVisible(bool visible);
};

class PartialPressureGasItem : public AbstractProfilePolygonItem {
	Q_OBJECT
public:
	PartialPressureGasItem(const DivePlotDataModel &model, const DiveCartesianAxis &hAxis, int hColumn, const DiveCartesianAxis &vAxis, int vColumn);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
	void replot() override;
	void setThresholdSettingsKey(const double *prefPointerMin, const double *prefPointerMax);
	void setVisibilitySettingsKey(const QString &setVisibilitySettingsKey);
	void setColors(const QColor &normalColor, const QColor &alertColor);

private:
	QVector<QPolygonF> alertPolygons;
	const double *thresholdPtrMin;
	const double *thresholdPtrMax;
	QString visibilityKey;
	QColor normalColor;
	QColor alertColor;
};
#endif // DIVEPROFILEITEM_H
