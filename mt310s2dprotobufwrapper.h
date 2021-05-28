#ifndef MT310S2DPROTOBUFWRAPPER_H
#define MT310S2DPROTOBUFWRAPPER_H

#include <xiqnetwrapper.h>

class cMT310S2dProtobufWrapper : public XiQNetWrapper
{
public:
    cMT310S2dProtobufWrapper();


    std::shared_ptr<google::protobuf::Message> byteArrayToProtobuf(QByteArray bA) override;
};

#endif // MT310S2DPROTOBUFWRAPPER_H
