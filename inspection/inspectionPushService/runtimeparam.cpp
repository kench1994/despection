#include "runtimeparam.h"
RuntimeParam * RuntimeParam::m_pSysParam = NULL;
RuntimeParam::RuntimeParam()
{
}

RuntimeParam::~RuntimeParam()
{
}

RuntimeParam* RuntimeParam::getInstance(){
    if(m_pSysParam == NULL)
        m_pSysParam = new RuntimeParam();
    return m_pSysParam;
}

QString RuntimeParam::getPadId(){
    if(m_nPadId.isEmpty()){
        CSystemDeviceManager sysDeviceMng;
        m_nPadId = sysDeviceMng.imei(0);
        if(m_nPadId.isEmpty())
            m_nPadId = "1111111111";
    }
    return m_nPadId;
}
