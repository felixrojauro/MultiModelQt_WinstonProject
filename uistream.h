#ifndef UISTREAM_H
#define UISTREAM_H

//#include <TStream.h>  My Thermodynamic Class
#include <QObject>
#include <QAbstractListModel>
#include <QString>
#include <QVector>
#include <vector>
#include <string>
#include <QHash>
#include <QDebug>
#include <Qt>
#include <deque>

class baseData : public QObject {   // base data for most models
    Q_OBJECT
public:
    baseData(QObject *parent = 0);
    ~baseData();
    baseData(const baseData &s);
    baseData& operator=(const baseData &s);

    bool isInput, isVisable, isReportable;
    int unitIndex;
    double value;
    QString type, tooltip;
	QStringList unitList;
};

class BaseModel : public QAbstractListModel{
    Q_OBJECT
public:
    enum BaseRoles {
        isInput = Qt::UserRole + 1,
        isVisable,
        isReportable,
        unitIndex,
        value,
        type,
        tooltip,
		unitList
	};
    explicit BaseModel(QObject *parent = 0);
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
	virtual QVariant data(int a_iRow, int role) const;
	bool setData( const QModelIndex& a_rIndex, const QVariant& a_rValue, int a_iRole );
	bool setData( int a_iRow, const QVariant& a_rValue, int a_iRole );
    QHash<int, QByteArray> roleNames() const;

//	// qabstractitemmodel subclass begin
//	virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
//	virtual QModelIndex parent(const QModelIndex &child) const;
//	virtual int columnCount(const QModelIndex &parent) const;
//	// qabstractitemmodel subclass end
	void clear();
	void append( baseData item );
	bool isEmpty() const;
    BaseModel(const BaseModel &m);
    BaseModel& operator =(const BaseModel &m);

private:
    QVector<baseData> model;
};

class UIStream : public QObject
{
    Q_OBJECT
	Q_PROPERTY(BaseModel* compbase READ compbase WRITE setCompbase NOTIFY compbaseChanged)
	Q_PROPERTY(BaseModel* compModel READ compModel WRITE setCompModel NOTIFY compModelChanged)
	Q_PROPERTY(BaseModel* propModel READ propModel WRITE setPropModel NOTIFY propModelChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)

public:
    UIStream(QObject *parent = 0);
    ~UIStream();
    UIStream(const UIStream &s);
//    TStream m_stream;         // My thermodynamic class
	BaseModel* m_compbase;    // Base model for all composition
	BaseModel* m_compModel;   // Model for composition
	BaseModel* m_propModel;   // Model for feed/combined properties
    QString m_Title;

    UIStream& operator=(const UIStream &m);

	Q_INVOKABLE void loadCompModel();   //Functions to populate the Composition Model
	Q_INVOKABLE void loadCompBaseModel();	//Functions to populate the composition base model
	Q_INVOKABLE void loadPropModel();   //Functions to populate the Property Model
    void UItoEngine();      //Send data from UI Model to Engine
    void EnginetoUI();      //Update UI from Engine
    double unitConversion_UI2E(QString type, double uivalue, int unitIndex);  // Convert UI values to engine basis
    double unitConversion_E2UI(QString type, int unitIndex);  // Convert Engine values to UI selected values

	BaseModel* compbase() const;
	void setCompbase(BaseModel *model);
	BaseModel* compModel();
	void setCompModel(BaseModel *model);
	BaseModel* propModel() const;
	void setPropModel(BaseModel *model);
    QString title() const;
    void setTitle(const QString &string);

	QStringList compUnitList();
	Q_INVOKABLE void setCompUnitIndex(int i);

private:

signals:
    void compbaseChanged();
    void compModelChanged();
    void propModelChanged();
    void titleChanged();
};

Q_DECLARE_METATYPE(baseData)
Q_DECLARE_METATYPE(QVector<baseData>)
Q_DECLARE_METATYPE(BaseModel)
Q_DECLARE_METATYPE(UIStream)
//Q_DECLARE_SEQUENTIAL_CONTAINER_METATYPE(baseData)
//Q_DECLARE_METATYPE(UIStream)
//qRegisterMetaType<BaseModel>("BaseModel");
//Q_DECLARE_SEQUENTIAL_CONTAINER_METATYPE(vector)
//Q_DECLARE_SEQUENTIAL_CONTAINER_METATYPE(BaseModel)
//Q_DECLARE_ASSOCIATIVE_CONTAINER_METATYPE(BaseModel)

#endif // UISTREAM_H
