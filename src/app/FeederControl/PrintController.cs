using CommandMessenger;
using CommandMessenger.TransportLayer;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FeederControl
{
    class PrintController
    {
        public static uint JobCompleteCount = 0;
        public static bool Ready = false;

        public PrintController(string comPort)
        {
            // Create Serial Port object
            // Note that for some boards (e.g. Sparkfun Pro Micro) DtrEnable may need to be true.
            _serialTransport = new SerialTransport
            {
                CurrentSerialSettings = { PortName = comPort, BaudRate = 115200, DtrEnable = false } // object initializer
            };

            // Initialize the command messenger with the Serial Port transport layer
            _cmdMessenger = new CmdMessenger(_serialTransport)
            {
                BoardType = BoardType.Bit16 // Set if it is communicating with a 16- or 32-bit Arduino board
            };

            _messages = new Messages(_cmdMessenger);

            AttachCommandCallBacks();

            // Attach to NewLinesReceived for logging purposes
            //_cmdMessenger.NewLineReceived += NewLineReceived;

            // Attach to NewLineSent for logging purposes
            //_cmdMessenger.NewLineSent += NewLineSent;

            // Start listening
            _cmdMessenger.StartListening();

            while (!Ready)
            {
                Console.WriteLine("Waiting for PrintBox to Connect");
                System.Threading.Thread.Sleep(1000);
            }
            // Send Connect message to Arduino, with unique id
            _messages.SendConnectCommand(6);
        }

        /// Attach command call backs. 
        private void AttachCommandCallBacks()
        {
            _cmdMessenger.Attach(OnUnknownCommand);
            _cmdMessenger.Attach((int)Messages.Command.JobCompleted, OnJobComplete);
            _cmdMessenger.Attach((int)Messages.Command.Error, OnError);
            _cmdMessenger.Attach((int)Messages.Command.Online, OnOnline);
            _cmdMessenger.Attach((int)Messages.Command.PrintMessage, OnPrintMessage);
        }

        // Callback function that the arduino is online and ready to connect
        void OnOnline(ReceivedCommand arguments)
        {
            Console.WriteLine("Feeder Online!");
            Ready = true;
        }

        // Callback function that prints a message from the arduino
        void OnPrintMessage(ReceivedCommand arguments)
        {
            var unique_id = arguments.ReadUInt16Arg();
            var box = arguments.ReadUInt16Arg();
            var message = arguments.ReadStringArg();
            Console.WriteLine("Controller {0}, Box:{1}, PrintMessage:{2}", unique_id, box, message);
            Ready = true;
        }

        // Called when a received command has no attached function.
        void OnUnknownCommand(ReceivedCommand arguments)
        {
            Console.WriteLine("Command without attached callback received");
        }

        // Callback function that prints that the Arduino has acknowledged
        void OnJobComplete(ReceivedCommand arguments)
        {
            JobCompleteCount++;
            var unique_id = arguments.ReadUInt16Arg();
            var box = arguments.ReadUInt16Arg();
            var fed = arguments.ReadUInt16Arg();
            Console.WriteLine("ID:{0} Job Complete on Box:{1}, Fed:{2}", unique_id, box, fed);
        }

        // Callback function that prints that the Arduino has experienced an error
        void OnError(ReceivedCommand arguments)
        {
            var unique_id = arguments.ReadUInt16Arg();
            var box = arguments.ReadUInt16Arg();
            var error = arguments.ReadUInt16Arg();
            Console.WriteLine("ID:{0} Error on Box:{1}, Error:{2:X}", unique_id, box, error);
        }

        public void Print(UInt16 box, UInt16 to_feed)
        {
            _messages.SendPrintCommand(box, to_feed);
        }

        // Log received line to console
        private void NewLineReceived(object sender, NewLineEvent.NewLineArgs e)
        {
            Console.WriteLine(@"Received > " + e.Command.CommandString());
        }

        // Log sent line to console
        private void NewLineSent(object sender, NewLineEvent.NewLineArgs e)
        {
            Console.WriteLine(@"Sent > " + e.Command.CommandString());
        }

        private CmdMessenger _cmdMessenger;
        private Messages _messages;
        private SerialTransport _serialTransport;
    }
}
