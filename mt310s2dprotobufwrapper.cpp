#include <qdebug.h>
#include <netmessages.pb.h>

#include "mt310s2dprotobufwrapper.h"


cMT310S2dProtobufWrapper::cMT310S2dProtobufWrapper()
{
}


google::protobuf::Message *cMT310S2dProtobufWrapper::byteArrayToProtobuf(QByteArray bA)
{
    ProtobufMessage::NetMessage *proto = new ProtobufMessage::NetMessage();
    if(!proto->ParseFromArray(bA, bA.size()))
    {
        ProtobufMessage::NetMessage::ScpiCommand *cmd = proto->mutable_scpi();
        cmd->set_command(bA.data(), bA.size() );
    }
    return proto;
}


QByteArray cMT310S2dProtobufWrapper::protobufToByteArray(google::protobuf::Message *pMessage)
{
    return QByteArray(pMessage->SerializeAsString().c_str(), pMessage->ByteSize());
}


