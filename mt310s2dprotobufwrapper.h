#ifndef MT310S2DPROTOBUFWRAPPER_H
#define MT310S2DPROTOBUFWRAPPER_H

#include <protonetwrapper.h>

class cMT310S2dProtobufWrapper : public ProtoNetWrapper
{
public:
  cMT310S2dProtobufWrapper();


  google::protobuf::Message *byteArrayToProtobuf(QByteArray bA);

  QByteArray protobufToByteArray(google::protobuf::Message *pMessage);
};

#endif // MT310S2DPROTOBUFWRAPPER_H
