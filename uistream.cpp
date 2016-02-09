#include "uistream.h"


baseData::baseData(QObject *parent) : QObject(parent) {}
baseData::~baseData() {}

baseData::baseData(const baseData &s) : QObject(s.parent() ){
    *this = s;
}

baseData& baseData::operator=(const baseData &s)
{
    isInput = s.isInput;
    isVisable = s.isVisable;
    isReportable = s.isReportable;
    unitIndex = s.unitIndex;
    value = s.value;
    type = s.type; tooltip = s.tooltip;
    unitList = s.unitList;
    return *this;
}

BaseModel::BaseModel(QObject *parent) :
    QAbstractListModel(parent)
{
    int i, n;
    std::string s;
//    TStream m_stream;
    n = 8; //n = m_stream.get_ncomp();   // Get number of components
    model.resize(n);
    for (i=0; i<n; i++) {
        s = "comp "; s+=i; //s = m_stream.get_abr(i);
        model[i].type = QString::fromStdString(s);
        model[i].value = 0.11; //model[i].value = m_stream.get_molfrac(i);
    }
    qDebug() << "BaseModel initialized";
}

BaseModel::BaseModel(const BaseModel &m) : QAbstractListModel(){
    this->model = m.model;
}

int BaseModel::rowCount(const QModelIndex &parent) const
{
    qDebug() << "BaseModel size is " << model.size();
    return model.size();
}

BaseModel& BaseModel::operator =(const BaseModel &m) {
    this->model = m.model;
    return *this;
}

QHash<int, QByteArray> BaseModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[isInput] = "isInput";
    roles[isVisable] = "isVisable";
    roles[isReportable] = "isReportable";
    roles[unitIndex] = "unitIndex";
    roles[value] = "value";
    roles[type] = "type";
    roles[tooltip] = "tooltip";
    roles[unitList] = "unitList";
    return roles;
}

QVariant BaseModel::data(const QModelIndex &index, int role) const{
    if(!index.isValid()) return QVariant();
    if(role == isInput)  return QVariant(model[index.row()].isInput);
    if(role == isVisable)  return QVariant(model[index.row()].isVisable);
    if(role == isReportable)  return QVariant(model[index.row()].isReportable);
    if(role == unitIndex)  return QVariant(model[index.row()].unitIndex);
    if(role == value)  return QVariant(model[index.row()].value);
    if(role == type)  return QVariant(model[index.row()].type);
    if(role == tooltip)  return QVariant(model[index.row()].tooltip);
    if(role == unitList)  return QVariant(model[index.row()].unitList);

    return QVariant();
}

bool BaseModel::setData(const QModelIndex& a_rIndex, const QVariant& a_rValue, int a_iRole)
{
	if ( a_rIndex.isValid() && a_iRole > Qt::UserRole )
	{
		baseData item = model.at( a_rIndex.row() );
		if(a_iRole == isInput)  item.isInput = a_rValue.toBool();
		else if(a_iRole == isVisable)  item.isVisable = a_rValue.toBool();
		else if(a_iRole == isReportable)  item.isReportable = a_rValue.toBool();
		else if(a_iRole == unitIndex)  item.unitIndex = a_rValue.toInt();
		else if(a_iRole == value)  item.value = a_rValue.toDouble();
		else if(a_iRole == type)  item.type = a_rValue.toString();
		else if(a_iRole == tooltip)  item.tooltip = a_rValue.toString();
		else if(a_iRole == unitList)  item.unitList = a_rValue.toStringList();
		else
		{
			return false;
		}
		emit dataChanged( a_rIndex, a_rIndex );
		return true;
	}
	return false;
}

//int UIStream::rowCount(const QModelIndex &parent) const
//{
//    return 3;
//}
//QVariant UIStream::data(const QModelIndex &index, int role) const
//{
//    if(!index.isValid()) return QVariant();
//    if(role == ecompbase)  return QVariant(m_compbase->model);
//    if(role == ecompModel)  return QVariant(m_compModel->model);
//    if(role == epropModel)  return QVariant(m_propModel->model);
//    return QVariant();
//}
//QHash<int, QByteArray> UIStream::roleNames() const
//{
//    QHash<int, QByteArray> roles;
//    roles[ecompbase] = "ecompbase";
//    roles[ecompModel] = "ecompModel";
//    roles[epropModel] = "epropModel";
//    return roles;
//}
UIStream& UIStream::operator=(const UIStream &m)
{
//    m_stream = m.m_stream;
    m_Title = m.m_Title;
    m_compbase = m.m_compbase;
    m_compModel = m.m_compModel;
    m_propModel = m.m_propModel;
    return *this;
}

UIStream::UIStream(QObject *parent) : QObject(parent), m_compbase( 0 ), m_compModel( 0 ), m_propModel( 0 )
{
    m_Title = "Stream title here";
//    loadCompModel();
//	loadCompBaseModel();
//    loadPropModel();
    qDebug() << "UIStream initiated";
}

UIStream::~UIStream()
{
	if ( m_compbase )
	{
		delete m_compbase;
		m_compbase = 0;
	}
	if ( m_compModel )
	{
		delete m_compModel;
		m_compModel = 0;
	}
	if ( m_propModel )
	{
		delete m_propModel;
		m_propModel = 0;
	}
}

UIStream::UIStream(const UIStream &s) : QObject(s.parent()), m_compbase( 0 ), m_compModel( 0 ), m_propModel( 0 ) {
//    m_stream = s.m_stream;
	if ( m_compbase && s.m_compbase )
	{
		*( m_compbase ) = *( s.m_compbase );
	}
	if ( m_compModel && s.m_compModel )
	{
		*( m_compModel ) = *( s.m_compModel );
	}
	if ( m_propModel && s.m_propModel )
	{
		*( m_propModel ) = *( s.m_propModel );
	}
    m_Title = s.m_Title;
}

void UIStream::loadCompModel()   //Functions to populate the Composition Model. Initialize only
{
    int i, n;
    std::string s;
    n = 8; //n = m_stream.get_ncomp();   // Get number of components
	if ( !m_compModel )
	{
		m_compModel = new BaseModel(this);
	}
	if ( m_compModel )
	{
		m_compModel->model.clear();
		m_compModel->model.resize(n);
		for (i=0; i<n; i++) {
			s = "comp "; s+=i; //s = m_stream.get_abr(i);
			m_compModel->model[i].type = QString::fromStdString(s);
			m_compModel->model[i].value = 0.11; //m_compModel->model[i].value = m_stream.get_molfrac(i);
		}
		emit compModelChanged();
	}
	qDebug() << Q_FUNC_INFO << " called";
}

void UIStream::loadCompBaseModel()
{
	if ( !m_compbase )
	{
		m_compbase = new BaseModel(this);
	}
	if ( m_compbase )
	{
		m_compbase->model.clear();
		m_compbase->model.resize(1);
		m_compbase->model[0].isInput = false;
		m_compbase->model[0].isVisable = false;
		m_compbase->model[0].isReportable = false;
		m_compbase->model[0].unitList << "mol frac" << "mol %" << "weight frac" << "weight %" << "lb-mol/hr" << "mols/hr" << "lb/hr" << "kg/hr";
		m_compbase->model[0].unitIndex = 0;
		m_compbase->model[0].value = 0;
		emit compbaseChanged();
	}
	qDebug() << Q_FUNC_INFO << " called";
}

void UIStream::loadPropModel()   //Functions to populate the Property Model
{
    QStringList units;
    int i = 0;
	if ( !m_propModel )
	{
		m_propModel = new BaseModel(this);
	}
	if ( m_propModel )
	{
		m_propModel->model.clear();
		m_propModel->model.resize(9);
		m_propModel->model[i].type = "P";
		m_propModel->model[i].tooltip = "Pressure";
		//m_propModel->model[i].value = m_stream.get_p();
		m_propModel->model[i].value = 14.69;
		m_propModel->model[i].unitIndex = 0;
		m_propModel->model[i].isInput = true;
		m_propModel->model[i].isVisable = true;
		m_propModel->model[i].isReportable = true;
		units << "psig" << "psia" << "atm" << "bar" << "kPa" << "inHg";
		m_propModel->model[i].unitList = units;

		units.clear();
		i=1;
		m_propModel->model[i].type = "T";
		m_propModel->model[i].tooltip = "Temperature";
		//m_propModel->model[i].value = m_stream.get_t();
		m_propModel->model[i].value = 60.0;
		m_propModel->model[i].unitIndex = 0;
		m_propModel->model[i].isInput = true;
		m_propModel->model[i].isVisable = true;
		m_propModel->model[i].isReportable = true;
		units << "F" << "R" << "C" << "K";
		m_propModel->model[i].unitList = units;

		units.clear();
		i=2;
		m_propModel->model[i].type = "Q";
		m_propModel->model[i].tooltip = "Flow Rate";
		//m_propModel->model[i].value = m_stream.get_qm();
		m_propModel->model[i].value = 100;
		m_propModel->model[i].unitIndex = 0;
		m_propModel->model[i].isInput = true;
		m_propModel->model[i].isVisable = true;
		m_propModel->model[i].isReportable = true;
		units << "MMscfd" << "lb/hr" << "lb-mols/hr" << "mols/hr";
		m_propModel->model[i].unitList = units;

		units.clear();
		i=3;
		m_propModel->model[i].type = "MW";
		m_propModel->model[i].tooltip = "Mole Weight";
		//m_propModel->model[i].value = m_stream.get_mw();
		m_propModel->model[i].value = 28.4;
		m_propModel->model[i].unitIndex = 0;
		m_propModel->model[i].isInput = false;
		m_propModel->model[i].isVisable = true;
		m_propModel->model[i].isReportable = true;
		units << "lb/lb-mol" << "g/mol" << "kg/kmol";
		m_propModel->model[i].unitList = units;

		units.clear();
		i=4;
		m_propModel->model[i].type = "z";
		m_propModel->model[i].tooltip = "Compressibility Factor";
		//m_propModel->model[i].value = m_stream.get_z();
		m_propModel->model[i].value = 0.83;
		m_propModel->model[i].unitIndex = 0;
		m_propModel->model[i].isInput = false;
		m_propModel->model[i].isVisable = true;
		m_propModel->model[i].isReportable = true;
		m_propModel->model[i].unitList = units;

		units.clear();
		i=5;
		m_propModel->model[i].type = "rho";
		m_propModel->model[i].tooltip = "Density";
		//m_propModel->model[i].value = m_stream.get_rho();
		m_propModel->model[i].value = 1.28;
		m_propModel->model[i].unitIndex = 0;
		m_propModel->model[i].isInput = false;
		m_propModel->model[i].isVisable = true;
		m_propModel->model[i].isReportable = true;
		units << "lb/ft^3" << "kg/m^3" << "lb/in^3";
		m_propModel->model[i].unitList = units;

		units.clear();
		i=6;
		m_propModel->model[i].type = "h";
		m_propModel->model[i].tooltip = "Enthalpy";
		//m_propModel->model[i].value = m_stream.get_h();
		m_propModel->model[i].value = 1230.4;
		m_propModel->model[i].unitIndex = 0;
		m_propModel->model[i].isInput = false;
		m_propModel->model[i].isVisable = true;
		m_propModel->model[i].isReportable = true;
		units << "Btu/lb" << "kJ/kg";
		m_propModel->model[i].unitList = units;

		units.clear();
		i=7;
		m_propModel->model[i].type = "s";
		m_propModel->model[i].tooltip = "Entropy";
		//m_propModel->model[i].value = m_stream.get_s();
		m_propModel->model[i].value = 1.5678;
		m_propModel->model[i].unitIndex = 0;
		m_propModel->model[i].isInput = false;
		m_propModel->model[i].isVisable = true;
		m_propModel->model[i].isReportable = true;
		units << "Btu/lb-F" << "kJ/kg-C";
		m_propModel->model[i].unitList = units;

		units.clear();
		i=8;
		m_propModel->model[i].type = "LHV";
		m_propModel->model[i].tooltip = "Lower Heating Value";
		//m_propModel->model[i].value = m_stream.get_lhv();
		m_propModel->model[i].value = 675.4;
		m_propModel->model[i].unitIndex = 0;
		m_propModel->model[i].isInput = false;
		m_propModel->model[i].isVisable = true;
		m_propModel->model[i].isReportable = true;
		units << "Btu/lb" << "kJ/kg";
		m_propModel->model[i].unitList = units;

		emit propModelChanged();
	}
	qDebug() << Q_FUNC_INFO << " called";
}

void UIStream::UItoEngine()      //Send data from UI Model to Engine
{
//    int i, n = m_stream.get_ncomp();
//    if (m_compbase->isInput) for (i=0; i<n; i++) {  // Loop through Composition setting the stream
//        m_stream.set_molfrac(i, m_compModel->model[i].value);
//    }
//    for (i=0; i<9; i++) if (m_propModel->model[i].isInput) { // Send inputs to Engine
//        unitConversion_UI2E(m_propModel->model[i].type, m_propModel->model[i].value, m_propModel->model[i].unitIndex);
//    }
}

void UIStream::EnginetoUI()      //Update UI from Engine
{
//    int i, n = m_stream.get_ncomp();
//    if (!m_compbase->isInput) for (i=0; i<n; i++) {  // Loop through Composition setting the stream
//        m_compModel->model[i].value = m_stream.get_molfrac(i);
//    }
//    double uivalue;
//    for (i=0; i<9; i++) if (!m_propModel->model[i].isInput) { // Get Engine Data if not input
//        uivalue = unitConversion_E2UI(m_propModel->model[i].type, m_propModel->model[i].unitIndex);
//        m_propModel->model[i].value = uivalue;
//    }
}

double UIStream::unitConversion_UI2E(QString type, double uivalue, int unitIndex)
// Convert units from theose selected in the UI to the base unit used int he engine
{
    double envalue; // Value going into engine
    if (type == "P") {
        switch (unitIndex) {
        case 0: envalue = uivalue + 14.6959; break; // psig
        case 1: envalue = uivalue;           break; // psia
        case 2: envalue = uivalue * 14.6959; break; // atm
        case 3: envalue = uivalue * 14.5038; break; // bar
        case 4: envalue = uivalue / 6.89476; break; // kPa
        case 5: envalue = uivalue / 2.03602; break; // inHg
        default: qDebug() << "The is NO pressure unit for unitIndex = " << unitIndex;
        }
        //m_stream.set_p(envalue);
    }
    else if(type == "T") { // Temperature
        switch (unitIndex) {
        case 0: envalue = uivalue + 459.67;               break; // F
        case 1: envalue = uivalue;                        break; // R
        case 2: envalue = (uivalue * 9.0 / 5.0) + 273.15; break; // C
        case 3: envalue = uivalue * 9.0 / 5.0;            break; // K
        default: qDebug() << "The is NO temperature unit for unitIndex = " << unitIndex;
        }
        //m_stream.set_t(envalue);
    }
    else if(type == "Q") { // Flow
        switch (unitIndex) {
//        case 0: m_stream.set_mm(uivalue);             break; // MMscfd
//        case 1: m_stream.set_mdothr(uivalue);         break; // lb/hr
//        case 2: m_stream.set_qm(uivalue);             break; // lb-mols/hr
//        case 3: m_stream.set_qm(uivalue * 453.59237); break; // mols/hr
//        default: qDebug() << "The is NO flow unit for unitIndex = " << unitIndex;
        }
    }
    else if(type == "MW") { // Mole Weight
        qDebug() << "Cannot set Mole Weight";
    }
    else if(type == "z") { // Compressibility Factor
        qDebug() << "Cannot set Compressibility Factor";
    }
    else if(type == "rho") { // Density
        switch (unitIndex) {
        case 0: envalue = uivalue;                        break; // lb/ft^3
        case 1: envalue = uivalue * 16.0185;              break; // kg/m^3
        case 2: envalue = uivalue * (12.0 * 12.0 * 12.0); break; // lb/in^3
        default: qDebug() << "The is NO density unit for unitIndex = " << unitIndex;
        }
        // No set function
    }
    else if(type == "h") { // Enthalpy
        switch (unitIndex) {
        case 0: envalue = uivalue;         break; // Btu/lb
        case 1: envalue = uivalue * 2.326; break; // kJ/kg
        default: qDebug() << "The is NO Enthalpy unit for unitIndex = " << unitIndex;
        }
        // No set function
    }
    else if(type == "s") { // Entropy
        switch (unitIndex) {
        case 0: envalue = uivalue;          break; // Btu/lb-F
        case 1: envalue = uivalue * 4.1868; break; // kJ/kg-C
        default: qDebug() << "The is NO Enthalpy unit for unitIndex = " << unitIndex;
        }
        // No set function
    }
    else if(type == "LHV") { // Lowwer Heating Value
        qDebug() << "Cannot set Lowwer Heating Value";
    }
    return envalue;
}

double UIStream::unitConversion_E2UI(QString type, int unitIndex)
// Convert units from base Engine units to those selected in the UI
{
    double envalue, uivalue;
    if(type == "P") { //Pressure
//        envalue = m_stream.get_p();
        switch (unitIndex) {
        case 0: uivalue = envalue - 14.6959; break; // psig
        case 1: uivalue = envalue;           break; // psia
        case 2: uivalue = envalue / 14.6959; break; // atm
        case 3: uivalue = envalue / 14.5038; break; // bar
        case 4: uivalue = envalue * 6.89476; break; // kPa
        case 5: uivalue = envalue * 2.03602; break; // inHg
        default: qDebug() << "The is NO pressure unit for unitIndex = " << unitIndex;
        }
    }
    else if(type == "T") { // Temperature
//        envalue = m_stream.get_t();
        switch (unitIndex) {
        case 0: uivalue = envalue - 459.67;             break; // F
        case 1: uivalue = envalue;                      break; // R
        case 2: uivalue = (envalue - 273.15) / 9.0 * 5.0; break; // C
        case 3: uivalue = envalue / 9.0 * 5.0;          break; // K
        default: qDebug() << "The is NO temperature unit for unitIndex = " << unitIndex;
        }
    }
    else if(type == "Q") { // Flow
        switch (unitIndex) {
//        case 0: uivalue = m_stream.get_mm();             break; // MMscfd
//        case 1: uivalue = m_stream.get_mdothr();         break; // lb/hr
//        case 2: uivalue = m_stream.get_qm();             break; // lb-mols/hr
//        case 3: uivalue = m_stream.get_qm() / 453.59237; break; // mols/hr
//        default: qDebug() << "The is NO flow unit for unitIndex = " << unitIndex;
        }
    }
    else if(type == "MW") { // Mole Weight
//        uivalue = m_stream.get_mw();
    }
    else if(type == "z") { // Compressibility Factor
//        uivalue = m_stream.get_z();
    }
    else if(type == "rho") { // Density
//        envalue = m_stream.get_rho();
        switch (unitIndex) {
        case 0: uivalue = envalue;                      break; // lb/ft^3
        case 1: uivalue = envalue / 16.0185;            break; // kg/m^3
        case 2: uivalue = envalue / (12.0 * 12.0 * 12.0); break; // lb/in^3
        default: qDebug() << "The is NO density unit for unitIndex = " << unitIndex;
        }
    }
    else if(type == "h") { // Enthalpy
//        envalue = m_stream.get_h();
        switch (unitIndex) {
        case 0: uivalue = envalue;         break; // Btu/lb
        case 1: uivalue = envalue / 2.326; break; // kJ/kg
        default: qDebug() << "The is NO Enthalpy unit for unitIndex = " << unitIndex;
        }
    }
    else if(type == "s") { // Entropy
//        envalue = m_stream.get_s();
        switch (unitIndex) {
        case 0: uivalue = envalue;          break; // Btu/lb-F
        case 1: uivalue = envalue / 4.1868; break; // kJ/kg-C
        default: qDebug() << "The is NO Enthalpy unit for unitIndex = " << unitIndex;
        }
    }
    else if(type == "LHV") { // Lowwer Heating Value
//        envalue = m_stream.get_lhv();
        switch (unitIndex) {
        case 0: uivalue = envalue;         break; // Btu/lb
        case 1: uivalue = envalue / 2.326; break; // kJ/kg
        default: qDebug() << "The is NO Enthalpy unit for unitIndex = " << unitIndex;
        }
    }
    return uivalue;
}


BaseModel* UIStream::compbase() const{ return m_compbase; }
void UIStream::setCompbase(BaseModel* model) {
	if ( model )
	{
		if ( m_compbase )
		{
			delete m_compbase;
		}
		m_compbase = model;
		emit compbaseChanged();
	}
}

BaseModel* UIStream::compModel() {
	if ( m_compModel )
	{
		qDebug() << "sent compModel.  Size here is " << m_compModel->model.size();
	}
	return m_compModel;
}
void UIStream::setCompModel(BaseModel* model) {
	if ( model )
	{
		if ( m_compModel )
		{
			delete m_compModel;
		}
		m_compModel = model;
		emit compModelChanged();
	}
}

BaseModel* UIStream::propModel() const { return m_propModel; }
void UIStream::setPropModel(BaseModel* model){
	if ( model )
	{
		if ( m_propModel )
		{
			delete m_propModel;
		}
		m_propModel = model;
		emit propModelChanged();
	}
}

QString UIStream::title() const{ return m_Title; }
void UIStream::setTitle(const QString &string){
	if ( m_Title != string )
	{
		m_Title = string;
		emit titleChanged();
	}
}

QStringList UIStream::compUnitList() { if ( m_compbase && !m_compbase->model.isEmpty() ) return m_compbase->model[0].unitList; else return QStringList(); }
void UIStream::setCompUnitIndex(int i) { if ( m_compbase && !m_compbase->model.isEmpty() ) m_compbase->model[0].unitIndex = i;  emit compbaseChanged(); }
