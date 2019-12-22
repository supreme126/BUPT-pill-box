/**********************************************************************
 *  Copyright(c) 2014-2015 China Mobile M2M Company Limited.
 *  All rights reserved.
 *
 *  �ļ�����: HttpPacket.cpp
 *  ��Ҫ����: Arduino http packets encapsulation library
 *
 *  ��������: 2015.4.24
 *  ����: Zhou Jiaxu
 *  ˵��:  Arduino http packets encapsulation library
 *  �汾:  V2.0
 *
 *  �޸�����:2015.5.29
 *  ����:Zhou Jiaxu
 *  ˵��:�����V1.0�޸Ĺ���������ѯ�������ĺ��������
 *               SDKʹ��ʾ��
 *
 *  �޸�����:2015.10.30
 *  ����:Zhou Jiaxu
 *  ˵��:�����V2.0�޸�
 *              int createCmdPacket(OPS_TYPE operType, RESRC_TYPE type, JsonObject&  jsonData);
 *                                        Ϊ 
 *              int createCmdPacket(OPS_TYPE operType, RESRC_TYPE type, char* jsonData);
 *              V2.0ʹ��JSON��, �����ڴ�ռ�ù��󣬿��ܵ��³���
 *              ������V2.1ֱ��ʹ�÷�װ�õ�JSON����Ϊ���룬��ʡ
 *              �ڴ濪����������ϡ�
 **********************************************************************/

#include <HttpPacket.h>

/**********************************************************************
 *  HttpPacketHead::HttpPacketHead(void)
 *  ��������: ���캯��
 *
 *  @return
 **********************************************************************/
HttpPacketHead::HttpPacketHead(void)
{
    clearUrlParameter();
    memset(content, 0, CMDLEN);

    apiKey = NULL;
    accessKey = NULL;
    hostAddress = NULL;
    devId = NULL;
    dataStreamId = NULL;
    triggerId = NULL;
    index = NULL;
    urlParaCnt = 0;

    for(int i = 0; i < URLPAPACNT; i++)
    {
        urlParaArr[i] = NULL;
    }

}

/**********************************************************************
 *  void HttpPacketHead::setHostAddress(char* hostAddr)
 *  ��������: ����http��ͷ������ַ
 *  @param param1   char* hostAddr  ��������ַ�ַ���ָ��
 *
 *  @return  void
 **********************************************************************/
void HttpPacketHead::setHostAddress(char* hostAddr)
{
    hostAddress = hostAddr;
    return;
}

/**********************************************************************
 *  void HttpPacketHead::updateUrlParameterWith(char* key, char* value)
 *  ��������: ���µ���URL����
 *  @param param1   char* key      ���������ַ���ָ��
 *  @param param2   char* value    ������ֵ�ַ���ָ��
 *  @return  void
 **********************************************************************/
void HttpPacketHead::updateUrlParameterWith(char* key, char* value)
{
    int tmp = urlParaIsExist(key);
    char* pKey = NULL;

    if(RET_ERR != tmp)
    {
        if((pKey = (char*)malloc(strlen(key) + strlen(value) + 5))  != NULL)
        {
            sprintf(pKey, "%s=%s", key, value);
            urlParaArr[tmp] = pKey;
        }
    }
}

/**********************************************************************
 * void HttpPacketHead::clearUrlParameter(void)
 *  ��������: �������֮ǰ���õ�URL����
 *  @param void
 *  @return  void
 **********************************************************************/
void HttpPacketHead::clearUrlParameter(void)
{
    for(int i = 0; i < URLPAPACNT; i++)
    {
        free(urlParaArr[i]);
        urlParaArr[i] = NULL;
    }
    urlParaCnt = 0;
}

/**********************************************************************
 * void HttpPacketHead::removeUrlParameter(char* key)
 *  ��������:ɾ��һ��֮ǰ�Ѿ����ڵ�URL���������û����
 *                        ������ �����κβ���
 *  @param char* key   ��������
 *  @return  void
 **********************************************************************/
void HttpPacketHead::removeUrlParameter(char* key)
{
    int i;
    int tmp = urlParaIsExist(key);

    if(RET_ERR != tmp)
    {
        free(urlParaArr[tmp]);
        urlParaArr[tmp] = NULL;
        for(i = tmp; i < urlParaCnt - 1; i++)
        {
            urlParaArr[i] = urlParaArr[i + 1];
        }
        urlParaArr[i] = NULL;
        urlParaCnt--;
    }
}

/**********************************************************************
 * int HttpPacketHead::urlParaIsExist(char* key)
 *  ��������:�ж�http�����Ƿ����ĳ��url����
 *  @param char* key   ��������
 *  @return   RET_ERR   ����������
 *                  i            �������ڣ������ز�����index
 **********************************************************************/
int HttpPacketHead::urlParaIsExist(char* key)
{
    int i;
    char buf[20];

    sprintf(buf, "%s=", key);

    for(i = 0; i < urlParaCnt; i++)
    {
        if(strlen(urlParaArr[i]) && strstr(urlParaArr[i], buf) != NULL)
        {
            break;
        }
    }

    if(urlParaCnt <= i) //�²���
    {
        return RET_ERR;
    }
    else
    {
        return i;
    }
}

/**********************************************************************
 * void HttpPacketHead::addUrlParameter(char* key, char* value)
 *  ��������:���һ��url ����http���У�������ڸò�����
 *                       �����κβ���.
 *  @param1 char* key       ��������
 *  @param2 char* value     ����ֵ
 *  @return   void
 **********************************************************************/
void HttpPacketHead::addUrlParameter(char* key, char* value)
{
    int i = 0, len = 0;
    char buf[20];
    char tmp[20];
    char* p;

    sprintf(buf, "%s=", key);

    if(urlParaCnt >= URLPAPACNT)
    {
        return;
    }

    for(i = 0; i < urlParaCnt; i++)
    {
        if(strlen(urlParaArr[i]) && strstr(urlParaArr[i], buf) != NULL)
        {
            break;
        }
    }

    if(strstr(urlParaArr[i], "datastream_ids=") != NULL) //���в���
    {
        len = strlen(urlParaArr[i]) - strlen(value);
        p = urlParaArr[i];
        p = p + len - 2;
        
        strcpy(tmp, p);
        sprintf(buf, ",%s", value);
        if(0 != strcmp(tmp, buf))
        {
            sprintf(buf, "=%s,", value);
            if(NULL == strstr(urlParaArr[i], buf))
            {
                sprintf(buf, ",%s,", value);
                if(NULL == strstr(urlParaArr[i], buf))
                {
                    strcat(urlParaArr[i] , ",");
                    strcat(urlParaArr[i] , value);
                }
            }
        }
    }

    if(urlParaCnt <= i && urlParaCnt < URLPAPACNT) //�²���
    {
        if(strcmp(key, "datastream_ids") == 0)
        {
            urlParaArr[i] = (char*)malloc(50);
        }
        else
        {
            urlParaArr[i] = (char*)malloc(strlen(key) + strlen(value) + 5);
        }

        if(NULL != urlParaArr[i])
        {
            sprintf(urlParaArr[i], "%s=%s", key, value);
        }
        urlParaCnt++;
    }
}

/**********************************************************************
 * void HttpPacketHead::setDevId(char* Dev_Id)
 *  ��������:����http����ҪҪ���ʵ��豸id
 *  @param1 char* Dev_Id       �豸ID �ַ���
 *  @return   void
 **********************************************************************/
void HttpPacketHead::setDevId(char* Dev_Id)
{
    devId = Dev_Id;
    return;
}

/**********************************************************************
 * void HttpPacketHead::setDataStreamId(char* StreamId)
 *  ��������:����http����ҪҪ���ʵ�������ID
 *  @param1 char* StreamId      ������ID �ַ���
 *  @return   void
 **********************************************************************/
void HttpPacketHead::setDataStreamId(char* StreamId)
{
    dataStreamId = StreamId;
    return;
}

/**********************************************************************
 * void HttpPacketHead::setTriggerId(char* TrigId)
 *  ��������:����http����ҪҪ���ʵĴ�����ID
 *  @param1 char* TrigId      ������ID �ַ���
 *  @return   void
 **********************************************************************/
void HttpPacketHead::setTriggerId(char* TrigId)
{
    triggerId = TrigId;
    return;
}

/**********************************************************************
 * void HttpPacketHead::setBinIdx(char* Idx)
 *  ��������:����http����ҪҪ���ʵ�bin�ļ�����
 *  @param1 char* Idx    �ļ�����
 *  @return   void
 **********************************************************************/
void HttpPacketHead::setBinIdx(char* Idx)
{
    index = Idx;
    return;
}

/**********************************************************************
 * void HttpPacketHead::addAPIKey(char* key)
 *  ��������:���һ��API key��ֻ����API ���²����вŻ�����.
 *  @param1 char* key    key�ַ���
 *  @return   void
 **********************************************************************/
void HttpPacketHead::addAPIKey(char* key)
{
    apiKey = key;
    return;
}

/**********************************************************************
 * void HttpPacketHead::setAccessKey(char* key)
 *  ��������:���÷����豸�������Կ.
 *  @param1 char* key    key�ַ���
 *  @return   void
 **********************************************************************/
void HttpPacketHead::setAccessKey(char* key)
{
    accessKey = key ;
    return;
}

/**********************************************************************
 * void HttpPacketHead::productGetUrl(RESRC_TYPE Type)
 *  ��������:������ѯ��������url����
 *  @param1 RESRC_TYPE Type     ��Դ����
 *  @return   void
 **********************************************************************/
void HttpPacketHead::productGetUrl(RESRC_TYPE Type)
{
    int len = 0;

    strcpy(content, "GET ");
    len = strlen(content);

    switch(Type)
    {
        case TYPE_USER:
            strcat(content, "/user");
            break;

        case TYPE_DEV:
            strcat(content, "/devices/");
            strcat(content, devId);
            break;

        case TYPE_DATASTREAM:
            if (RET_ERR != urlParaIsExist("datastream_ids"))
            {
                strcat(content, "/devices/");
                strcat(content, devId);
                strcat(content, "/datastreams");
             }
            else
            {
                strcat(content, "/devices/");
                strcat(content, devId);
                strcat(content, "/datastreams/");
                strcat(content, dataStreamId);
            }
            break;

        case TYPE_DATAPOINT:
            strcat(content, "/devices/");
            strcat(content, devId);
            strcat(content, "/datapoints");
            break;

        case TYPE_TRIGGER:
            strcat(content, "/triggers/");
            strcat(content, triggerId);
            break;

        case TYPE_APIKEY:
            strcat(content, "/keys");
            break;

        case TYPE_BINDATA:
            strcat(content, "/bindata/");
            strcat(content, index);
            break;

        case TYPE_APILOG:
            strcat(content, "/logs/");
            strcat(content, devId);
            break;

        case TYPE_HISTORYDATA:
            strcat(content, "/datapoints");
            break;

        default:
            break;
    }

    if(strlen(content) > len)
    {
        urlParaToCmd();
    }
    else
    {
        memset(content, 0, sizeof(content));
    }

}


/**********************************************************************
 * void HttpPacketHead::productPostUrl(RESRC_TYPE Type)
 *  ��������:����POST��������url����
 *  @param1 RESRC_TYPE Type     ��Դ����
 *  @return  void
 **********************************************************************/
void HttpPacketHead::productPostUrl(RESRC_TYPE Type)
{
    int len = 0;

    strcpy(content, "POST ");
    len = strlen(content);

    switch(Type)
    {
        case TYPE_DEV:
            strcat(content, "/devices");
            break;

        case TYPE_DATASTREAM:
            strcat(content, "/devices/");
            strcat(content, devId);
            strcat(content, "/datastreams");
            break;

        case TYPE_DATAPOINT:
            strcat(content, "/devices/");
            strcat(content, devId);
            strcat(content, "/datapoints");
            break;

        case TYPE_TRIGGER:
            strcat(content, "/triggers");
            break;

        case TYPE_APIKEY:
            strcat(content, "/keys");
            break;

        case TYPE_BINDATA:
            strcat(content, "/bindata");
            break;

        default:
            break;
    }

    if(strlen(content) > len)
    {
        urlParaToCmd();
    }
    else
    {
        memset(content, 0, sizeof(content));
    }
}


/**********************************************************************
 * void HttpPacketHead::productPutUrl(RESRC_TYPE Type)
 *  ��������:����PUT��������url����
 *  @param1 RESRC_TYPE Type     ��Դ����
 *  @return   void
 **********************************************************************/
void HttpPacketHead::productPutUrl(RESRC_TYPE Type)
{
    int len = 0;

    strcpy(content, "PUT ");
    len = strlen(content);

    switch(Type)
    {
        case TYPE_DEV:
            strcat(content, "/devices/");
            strcat(content, devId);
            break;

        case TYPE_DATASTREAM:
            strcat(content, "/devices/");
            strcat(content, devId);
            strcat(content, "/datastreams/");
            strcat(content, dataStreamId);
            break;

        case TYPE_TRIGGER:
            strcat(content, "/triggers/");
            strcat(content, triggerId);
            break;

        case TYPE_APIKEY:
            strcat(content, "/keys/");
            strcat(content, apiKey);
            break;

        default:
            break;
    }

    if(strlen(content) > len)
    {
        urlParaToCmd();
    }
    else
    {
        memset(content, 0, sizeof(content));
    }
}

/**********************************************************************
 * void HttpPacketHead::productDeleteUrl(RESRC_TYPE Type)
 *  ��������:����DELETE��������url����
 *  @param1 RESRC_TYPE Type     ��Դ����
 *  @return   void
 **********************************************************************/
void HttpPacketHead::productDeleteUrl(RESRC_TYPE Type)
{
    int len = 0;

    strcpy(content, "DELETE ");
    len = strlen(content);

    switch(Type)
    {
        case TYPE_DEV:
            strcat(content, "/devices/");
            strcat(content, devId);
            break;

        case TYPE_DATASTREAM:
            strcat(content, "/devices/");
            strcat(content, devId);
            strcat(content, "/datastreams/");
            strcat(content, dataStreamId);
            break;

        case TYPE_DATAPOINT:
            strcat(content, "/devices/");
            strcat(content, devId);
            strcat(content, "/datapoints");
            break;

        case TYPE_TRIGGER:
            strcat(content, "/triggers/");
            strcat(content, triggerId);
            break;

        case TYPE_APIKEY:
            strcat(content, "/keys/");
            strcat(content, apiKey);
            break;

        case TYPE_BINDATA:
            strcat(content,  "/bindata/");
            strcat(content, index);
            break;
        default:
            break;
    }

    if(strlen(content) > len)
    {
        urlParaToCmd();
    }
    else
    {
        memset(content, 0, sizeof(content));
    }
}

/**********************************************************************
 * void HttpPacketHead::productJsonLen(int jsonLen)
 *  ��������:����http ��Content-Length:�ֶ�����
 *  @param1 int jsonLen     http������json ���ݳ���
 *  @return   void
 **********************************************************************/
void HttpPacketHead::productJsonLen(int jsonLen)
{
    char buff[30];
    
    sprintf(buff, "Content-Length:%d\r\n\r\n", jsonLen);
    strcat(content, buff);
}

/**********************************************************************
void HttpPacketHead::urlParaToCmd(void)
 *  ��������:���url�����������
 *  @param1 void
 *  @return   void
 **********************************************************************/
void HttpPacketHead::urlParaToCmd(void)
{
    int i =0;

    if(urlParaCnt > 0)
    {
        strcat(content, "?");
        strcat(content, urlParaArr[0]);
        for(i = 1; i < urlParaCnt; i++)
        {
            strcat(content, "&");
            strcat(content, urlParaArr[i]);
        }
    }

    strcat(content, " HTTP/1.1\r\n");
}

/**********************************************************************
int HttpPacketHead::createCmdPacket(OPS_TYPE operType, RESRC_TYPE type, char* jsonData)
 *  ��������:����http ���ݰ�
 *  @param1 OPS_TYPE     operType          ��������
 *  @param2 RESRC_TYPE  type                ��Դ����
 *  @param3 char* jsonData                    http����json�ṹ
 *  @return   RET_ERR      �������ݰ�ʧ�ܣ�������Ϊ�������벻��
                  RET_OK      �������ݰ��ɹ�
 **********************************************************************/
int HttpPacketHead::createCmdPacket(OPS_TYPE operType, RESRC_TYPE type, char* jsonData)
{
    int jsonLen = strlen(jsonData);
    char buf[50];

    memset(content, 0, CMDLEN);
    memset(buf, 0, sizeof(buf));
    switch(operType)
    {
        case POST:
            productPostUrl(type);
            if(strlen(content))
            {
                sprintf(buf, "api-key:%s\r\n", accessKey);
                strcat(content, buf);
                sprintf(buf, "Host:%s\r\n", hostAddress);
                strcat(content, buf);
                productJsonLen(jsonLen);
            }
            break;

        case PUT:
            productPutUrl(type);
            if(strlen(content))
            {
                sprintf(buf, "api-key:%s\r\n", accessKey);
                strcat(content, buf);
                sprintf(buf, "Host:%s\r\n", hostAddress);
                strcat(content, buf);
                productJsonLen(jsonLen);
            }
            break;

        default:
            break;
    }

    if(strlen(content))
    {
        return RET_OK;
    }
    else
    {
        return RET_ERR;
    }
}

/**********************************************************************
int HttpPacketHead::createCmdPacket(OPS_TYPE operType, RESRC_TYPE type)
 *  ��������:����http ���ݰ�
 *  @param1 OPS_TYPE     operType          ��������
 *  @param2 RESRC_TYPE  type                ��Դ����
 *  @return   RET_ERR      �������ݰ�ʧ�ܣ�������Ϊ�������벻��
                  RET_OK      �������ݰ��ɹ�
 **********************************************************************/
int HttpPacketHead::createCmdPacket(OPS_TYPE operType, RESRC_TYPE type)
{
    char buf[50];

    memset(content, 0, CMDLEN);
    memset(buf, 0, sizeof(buf));
    switch(operType)
    {
        case GET:
            productGetUrl(type);
            if(strlen(content))
            {
                sprintf(buf, "api-key:%s\r\n", accessKey);
                strcat(content, buf);
                sprintf(buf, "Host:%s\r\n\r\n", hostAddress);
                strcat(content, buf);
            }
            break;

        case DELETE:
            productDeleteUrl(type);
            if(strlen(content))
            {
                sprintf(buf, "api-key:%s\r\n", accessKey);
                strcat(content, buf);
                sprintf(buf, "Host:%s\r\n\r\n", hostAddress);
                strcat(content, buf);
            }
            break;

        default:
            break;
    }

    if(strlen(content))
    {
        return RET_OK;
    }
    else
    {
        return RET_ERR;
    }
}

/**********************************************************************
HttpPacketHead::~HttpPacketHead(void)
 *  ��������:��������
 *  @param1
 *  @return
 **********************************************************************/
HttpPacketHead::~HttpPacketHead(void)
{
    for(int i = 0; i < URLPAPACNT; i++)
    {
        free(urlParaArr[i]);
        urlParaArr[i]= NULL;
    }

}
