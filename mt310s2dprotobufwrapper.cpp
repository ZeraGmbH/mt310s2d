#include <qdebug.h>
#include <netmessages.pb.h>

#include "mt310s2dprotobufwrapper.h"


cMT310S2dProtobufWrapper::cMT310S2dProtobufWrapper()
{
}


std::shared_ptr<google::protobuf::Message> cMT310S2dProtobufWrapper::byteArrayToProtobuf(QByteArray bA)
{
    ProtobufMessage::NetMessage *intermediate = new ProtobufMessage::NetMessage();
    if(!intermediate->ParseFromArray(bA, bA.size()))
    {
        ProtobufMessage::NetMessage::ScpiCommand *cmd = intermediate->mutable_scpi();
        cmd->set_command(bA.data(), bA.size() );
    }
    std::shared_ptr<google::protobuf::Message> proto {intermediate};
    return proto;
}


QByteArray cMT310S2dProtobufWrapper::protobufToByteArray(const google::protobuf::Message &pMessage)
{
    return QByteArray(pMessage.SerializeAsString().c_str(), pMessage.ByteSizeLong());
}


