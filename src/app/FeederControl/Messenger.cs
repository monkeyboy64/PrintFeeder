using CommandMessenger;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FeederControl
{
    class Messages
    {
        public enum Command : int
        {
            // Send Messages
            Print,                  // Receive, with box and to_feed amount
            Connect,              // Receive, with a unique id, acknowledge with kAttach

            // Receive Messages.  All messages are have a unique printer id
            AcknowledgeConnect,   // Send, unique_id, when connected, send back unique id
            AcknowledgePrint,       // Send, box and to_feed amount, Acknowledge that we received a print job
            Error,                  // Send, box and error, There was an error
            JobCompleted,           // Send, box and fed, when a print job is completed,
            Online,
            PrintMessage,
            UnknownCommand,
        };

        public Messages(CmdMessenger cmdMessenger)
        {
            _cmdMessenger = cmdMessenger;
        }

        public void SendPrintCommand(UInt16 box, UInt16 to_feed)
        {
            Console.WriteLine("Sending Print command to box {0} and feed {1}", box, to_feed);
            // Send the command, wait for 1s for the the acknowledge command
            SendCommand command = new SendCommand((int) Command.Print, (int) Command.AcknowledgePrint, 1000);
            command.AddArgument(box);
            command.AddArgument(to_feed);

            ReceivedCommand rec_command = _cmdMessenger.SendCommand(command);

            var unique_id = rec_command.ReadUInt16Arg();
            box = rec_command.ReadUInt16Arg();
            to_feed = rec_command.ReadUInt16Arg();

            Console.WriteLine("Acknowledge Print command with unique id {0} to box {1} and feed {2}", unique_id, box, to_feed);
        }

        public void SendConnectCommand(UInt16 box_id)
        {
            Console.WriteLine("Sending Connect command with unique id {0}", box_id);

            // Send the command, wait for 1s for the the acknowledge command
            SendCommand command = new SendCommand((int) Command.Connect, box_id, (int) Command.AcknowledgeConnect, 1000);
            _cmdMessenger.SendCommand(command);

            ReceivedCommand rec_command = _cmdMessenger.SendCommand(command);

            if (rec_command.Ok)
            {
                var unique_id = rec_command.ReadUInt16Arg();

                Console.WriteLine("Acknowledge Connect command with unique id {0}", unique_id);
            }
            else
            {
                Console.WriteLine("Bad messages");
            }

        }

        private CmdMessenger _cmdMessenger;
    }
}
