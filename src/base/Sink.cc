//
// Copyright (C) 1992-2004 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include "Sink.h"


Define_Module(Sink);

simsignal_t Sink::rcvdPkSignal = SIMSIGNAL_NULL;

void Sink::initialize()
{
    numPackets = 0;
    numBits = 0;
    throughput = 0;
    packetPerSec = 0;
    rcvdPkSignal = registerSignal("rcvdPk");

    WATCH(numPackets);
    WATCH(numBits);
    WATCH(throughput);
    WATCH(packetPerSec);
}

void Sink::handleMessage(cMessage *msg)
{
    numPackets++;
    cPacket *packet = PK(msg);
    numBits += packet->getBitLength();
    emit(rcvdPkSignal, packet);
    throughput = numBits / simTime();
    packetPerSec = numPackets / simTime();

    delete msg;
}

void Sink::finish()
{
    recordScalar("throughput", throughput);
    recordScalar("packetPerSec", packetPerSec);
}


