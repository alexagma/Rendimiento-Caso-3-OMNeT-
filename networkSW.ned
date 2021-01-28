//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

import ned.DatarateChannel;

simple Source
{
    parameters:
        @display("i=block/source");
        int lambda = 2;
        int samples = 100;
    gates:
        output out;
}

simple Sender
{
    parameters:
        double probability;
        @display("i=block/departure");
    gates:
        input in[];
        output out[];
}

simple Receiver
{
    parameters:
        @display("i=block/sink");
    gates:
        input in[];
        output out[];
}

network networkSW
{
    @display("bgb=426.87,312.47998");
    types:
        channel Channel extends DatarateChannel
        {
            datarate = 9600bps;
            delay = 100us;
            per = 0.05;
        }
    submodules:

        Source1: Source {
            @display("p=31.619999,152.51999");
        }
        Source2: Source {

            @display("p=137,38");
        }
        Source3: Source {

            @display("p=137,262");
        }
        node1: Sender {
            @display("p=98.579994,152.51999");
            probability = 1/4;
            gates:
                in[3];
                out[2];
        }
        node2: Sender {
            @display("p=204.59999,102.299995");
            probability = 1/3;
            gates:
                in[5];
                out[4];
        }
        node5: Sender {
            @display("p=204.59999,216.68999");
            probability = 1/2;
            gates:
                in[4];
                out[3];
        }
        node3: Receiver {
            @display("p=365.49,102.299995");
            gates:
                in[1];
                out[1];
        }
        node4: Receiver {
            @display("p=365.49,216.68999");
            gates:
                in[2];
                out[2];
        }
    connections:
        Source1.out --> node1.in[2];
        Source2.out --> node2.in[4];
        Source3.out --> node5.in[3];

        node1.out[0] --> Channel --> node2.in[2];
        node1.in[0] <-- Channel <-- node2.out[2];

        node1.out[1] --> Channel --> node5.in[2];
        node1.in[1] <-- Channel <-- node5.out[2];

        node2.out[0] --> Channel --> node3.in[0];
        node2.in[0] <-- Channel <-- node3.out[0];

        node2.out[1] --> Channel --> node4.in[0];
        node2.in[1] <-- Channel <-- node4.out[0];

        node5.out[0] --> Channel --> node2.in[3];
        node5.in[0] <-- Channel <-- node2.out[3];

        node5.out[1] --> Channel --> node4.in[1];
        node5.in[1] <-- Channel <-- node4.out[1];

}
