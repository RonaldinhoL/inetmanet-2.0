//
// Copyright (C) 2008-2009 Irene Ruengeler
// Copyright (C) 2009-2012 Thomas Dreibholz
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//


#include "SCTPMessage.h"
#include "SCTPAssociation.h"


Register_Class(SCTPMessage);

SCTPMessage& SCTPMessage::operator=(const SCTPMessage& other)
{
     if (this == &other) return *this;
     clean();
     SCTPMessage_Base::operator=(other);
     copy(other);
     return *this;
}

void SCTPMessage::copy(const SCTPMessage& other)
{
     this->setTag(other.getTag());
     for (std::list<cPacket*>::const_iterator i=other.chunkList.begin(); i!=other.chunkList.end(); ++i)
          addChunk((cPacket *)(*i)->dup());
}

SCTPMessage::~SCTPMessage()
{
    clean();
}

void SCTPMessage::clean()
{
    SCTPChunk* chunk;
    if (this->getChunksArraySize()>0)
    for (uint32 i=0; i < this->getChunksArraySize(); i++)
    {
        chunk = (SCTPChunk*)this->getChunks(i);
        dropAndDelete(chunk);
    }
}

void SCTPMessage::setChunksArraySize(uint32 size)
{
     throw new cException(this, "setChunkArraySize() not supported, use addChunk()");
}

uint32 SCTPMessage::getChunksArraySize() const
{
     return chunkList.size();
}

cPacketPtr& SCTPMessage::getChunks(uint32 k)
{
     std::list<cPacket*>::iterator i = chunkList.begin();
     while (k>0 && i!=chunkList.end())
          (++i, --k);
     return *i;
}

void SCTPMessage::setChunks(uint32 k, const cPacketPtr& chunks_var)
{
     throw new cException(this, "setChunks() not supported, use addChunk()");
}

void SCTPMessage::addChunk(cPacket* msg)
{
    char str[256];
    take(msg);
    if (this->chunkList.size()<9)
    {
        strcpy(str, this->getName());
        snprintf(str, sizeof(str), "%s %s", this->getName(), msg->getName());
        this->setName(str);
    }
    this->setBitLength(this->getBitLength()+ADD_PADDING(msg->getBitLength()/8)*8);
    chunkList.push_back(msg);
}

cPacket *SCTPMessage::removeChunk()
{
    if (chunkList.empty())
        return NULL;

    cPacket *msg = chunkList.front();
    chunkList.pop_front();
    drop(msg);
    this->setBitLength(this->getBitLength()-ADD_PADDING(msg->getBitLength()/8)*8);
    return msg;
}

cPacket *SCTPMessage::removeLastChunk()
{
    if (chunkList.empty())
        return NULL;

    cPacket *msg = chunkList.back();
    chunkList.pop_back();
    drop(msg);
    this->setBitLength(this->getBitLength()-ADD_PADDING(msg->getBitLength()/8)*8);
    return msg;
}

cPacket *SCTPMessage::peekFirstChunk()
{
    if (chunkList.empty())
        return NULL;

    cPacket *msg = chunkList.front();
    return msg;
}

cPacket *SCTPMessage::peekLastChunk()
{
    if (chunkList.empty())
        return NULL;

    cPacket *msg = chunkList.back();
    return msg;
}


Register_Class(SCTPErrorChunk);

SCTPErrorChunk& SCTPErrorChunk::operator=(const SCTPErrorChunk& other)
{
     if (this == &other) return *this;
     clean();
     SCTPErrorChunk_Base::operator=(other);
     copy(other);
     return *this;
}

void SCTPErrorChunk::copy(const SCTPErrorChunk& other)
{
     for (std::list<cPacket*>::const_iterator i=other.parameterList.begin(); i!=other.parameterList.end(); ++i)
          addParameters((cPacket *)(*i)->dup());
}

void SCTPErrorChunk::setParametersArraySize(uint32 size)
{
     throw new cException(this, "setParametersArraySize() not supported, use addParameter()");
}

uint32 SCTPErrorChunk::getParametersArraySize() const
{
     return parameterList.size();
}

cPacketPtr& SCTPErrorChunk::getParameters(uint32 k)
{
     std::list<cPacket*>::iterator i = parameterList.begin();
     while (k>0 && i!=parameterList.end())
          (++i, --k);
     return *i;
}

void SCTPErrorChunk::setParameters(uint32 k, const cPacketPtr& chunks_var)
{
     throw new cException(this, "setParameter() not supported, use addParameter()");
}

void SCTPErrorChunk::addParameters(cPacket* msg)
{
     take(msg);

     this->setBitLength(this->getBitLength()+ADD_PADDING(msg->getBitLength()));
     parameterList.push_back(msg);
}

cPacket *SCTPErrorChunk::removeParameter()
{
    if (parameterList.empty())
        return NULL;

    cPacket *msg = parameterList.front();
    parameterList.pop_front();
    drop(msg);
    this->setBitLength(this->getBitLength()-ADD_PADDING(msg->getBitLength()/8)*8);
    return msg;
}

SCTPErrorChunk::~SCTPErrorChunk()
{
    clean();
}

void SCTPErrorChunk::clean()
{
    while (!parameterList.empty())
    {
        cPacket *msg = parameterList.front();
        parameterList.pop_front();
        dropAndDelete(msg);
    }
}

Register_Class(SCTPStreamResetChunk);

SCTPStreamResetChunk& SCTPStreamResetChunk::operator=(const SCTPStreamResetChunk& other)
{
    SCTPStreamResetChunk_Base::operator=(other);

    this->setBitLength(SCTP_STREAM_RESET_CHUNK_LENGTH*8);
    for (std::list<cPacket*>::const_iterator i=other.parameterList.begin(); i!=other.parameterList.end(); ++i)
        addParameter((cPacket *)(*i)->dup());

    return *this;
}

void SCTPStreamResetChunk::setParametersArraySize(const uint32 size)
{
    throw new cException(this, "setParametersArraySize() not supported, use addParameter()");
}

uint32 SCTPStreamResetChunk::getParametersArraySize() const
{
    return parameterList.size();
}

cPacketPtr& SCTPStreamResetChunk::getParameters(uint32 k)
{
    std::list<cPacket*>::iterator i = parameterList.begin();
    while (k>0 && i!=parameterList.end())
        (++i, --k);
    return *i;
}

void SCTPStreamResetChunk::setParameters(const uint32 k, const cPacketPtr& chunks_var)
{
    throw new cException(this, "setParameters() not supported, use addParameter()");
}

void SCTPStreamResetChunk::addParameter(cPacket* msg)
{
    take(msg);
    if (this->parameterList.size()<2)
    {
        this->setBitLength(this->getBitLength()+ADD_PADDING(msg->getBitLength()));
        parameterList.push_back(msg);
    }
    else
        throw cRuntimeError("Not more than two parameters allowed!");
}

cPacket *SCTPStreamResetChunk::removeParameter()
{
    if (parameterList.empty())
        return NULL;

    cPacket *msg = parameterList.front();
    parameterList.pop_front();
    drop(msg);
    this->setBitLength(this->getBitLength()-ADD_PADDING(msg->getBitLength()/8)*8);
    return msg;
}


Register_Class(SCTPAsconfChunk);

SCTPAsconfChunk& SCTPAsconfChunk::operator=(const SCTPAsconfChunk& other)
{
    SCTPAsconfChunk_Base::operator=(other);

    this->setBitLength(SCTP_ADD_IP_CHUNK_LENGTH*8);
    for (std::list<cPacket*>::const_iterator i=other.parameterList.begin(); i!=other.parameterList.end(); ++i)
        addAsconfParam((cPacket *)(*i)->dup());

    return *this;
}

void SCTPAsconfChunk::setAsconfParamsArraySize(const uint32 size)
{
    throw new cException(this, "setAsconfParamsArraySize() not supported, use addAsconfParam()");
}

uint32 SCTPAsconfChunk::getAsconfParamsArraySize() const
{
    return parameterList.size();
}

cPacketPtr& SCTPAsconfChunk::getAsconfParams(uint32 k)
{
    std::list<cPacket*>::iterator i = parameterList.begin();
    while (k>0 && i!=parameterList.end())
        (++i, --k);
    return *i;
}

void SCTPAsconfChunk::setAsconfParams(const uint32 k, const cPacketPtr& chunks_var)
{
    throw new cException(this, "setAsconfParams() not supported, use addAsconfParam()");
}

void SCTPAsconfChunk::addAsconfParam(cPacket* msg)
{
    take(msg);
    //if (this->parameterList.size()<2)
    //{
    this->setBitLength(this->getBitLength()+ADD_PADDING(msg->getBitLength()));
    parameterList.push_back(msg);
    /*}
   else
      throw cRuntimeError("Not more than two parameters allowed!");*/
}

cPacket *SCTPAsconfChunk::removeAsconfParam()
{
    if (parameterList.empty())
        return NULL;

    cPacket *msg = parameterList.front();
    parameterList.pop_front();
    drop(msg);
    this->setBitLength(this->getBitLength()-ADD_PADDING(msg->getBitLength()/8)*8);
    return msg;
}


Register_Class(SCTPAsconfAckChunk);

SCTPAsconfAckChunk& SCTPAsconfAckChunk::operator=(const SCTPAsconfAckChunk& other)
{
    SCTPAsconfAckChunk_Base::operator=(other);

    this->setBitLength(SCTP_ADD_IP_CHUNK_LENGTH*8);
    for (std::list<cPacket*>::const_iterator i=other.parameterList.begin(); i!=other.parameterList.end(); ++i)
        addAsconfResponse((cPacket *)(*i)->dup());

    return *this;
}

void SCTPAsconfAckChunk::setAsconfResponseArraySize(const uint32 size)
{
    throw new cException(this, "setAsconfResponseArraySize() not supported, use addAsconfResponse()");
}

uint32 SCTPAsconfAckChunk::getAsconfResponseArraySize() const
{
    return parameterList.size();
}

cPacketPtr& SCTPAsconfAckChunk::getAsconfResponse(uint32 k)
{
    std::list<cPacket*>::iterator i = parameterList.begin();
    while (k>0 && i!=parameterList.end())
        (++i, --k);
    return *i;
}

void SCTPAsconfAckChunk::setAsconfResponse(const uint32 k, const cPacketPtr& chunks_var)
{
    throw new cException(this, "setAsconfresponse() not supported, use addAsconfResponse()");
}

void SCTPAsconfAckChunk::addAsconfResponse(cPacket* msg)
{
    take(msg);
    /*if (this->parameterList.size()<2)
   {*/
    this->setBitLength(this->getBitLength()+ADD_PADDING(msg->getBitLength()));
    parameterList.push_back(msg);
    /*}
   else
      throw cRuntimeError("Not more than two parameters allowed!");*/
}

cPacket *SCTPAsconfAckChunk::removeAsconfResponse()
{
    if (parameterList.empty())
        return NULL;

    cPacket *msg = parameterList.front();
    parameterList.pop_front();
    drop(msg);
    this->setBitLength(this->getBitLength()-ADD_PADDING(msg->getBitLength()/8)*8);
    return msg;
}

