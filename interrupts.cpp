/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include<interrupts.hpp>

//Device Table has 0->20
//Vector Table has 0->26

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    
    const int context_save_time = 20;
    
    const int end_io_delay = 172;

    int current_time = 0;
    int ISR_activity_delay = 0;
    int ISR_activity_delay_2 = 0;
    int SYS_CALL_delay = 0;
    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/

        //execution += "Time of the event, Duration of the event, Event type"



        //SYS CALLS:
            //Switch to/from kernel mode (1ms)
            //Save/restore context (10ms)
            //Where in mem the ISR start address is (1ms)
            //Get ISR address from vector table (1ms)
            //Execute ISR body: Include all activities carried by ISR (each activity 40ms)
            //Execute IRET (1ms)




        //Vector Table: SYSCALL

        if(activity == "SYSCALL"){
            auto [execution_partial, time] = intr_boilerplate(current_time, duration_intr, context_save_time, vectors);

            execution += execution_partial;
            current_time = time;

            //Execute ISR body

            int delay_time = delays.at(duration_intr);

            if(delay_time <= 90){
                SYS_CALL_delay = delay_time / 5;
                ISR_activity_delay = (delay_time-SYS_CALL_delay) / 2;
                ISR_activity_delay_2 = delay_time - SYS_CALL_delay - ISR_activity_delay;
            }
            else{
                SYS_CALL_delay = delay_time - 80;
                ISR_activity_delay = 40;
                ISR_activity_delay_2 = 40;
            }

            execution += std::to_string(current_time) + ", " + std::to_string(SYS_CALL_delay) + ", call device driver\n";
            current_time += SYS_CALL_delay;

            //ISR Activity 1: Transfer data
            execution += std::to_string(current_time) + ", " + std::to_string(ISR_activity_delay) + ", Transfer data\n";
            current_time += ISR_activity_delay;
            //ISR Activity 2: Check for errors
            execution += std::to_string(current_time) + ", " + std::to_string(ISR_activity_delay_2) + ", Check for errors\n";
            current_time += ISR_activity_delay_2;

            //Execute IRET
            execution += std::to_string(current_time) + ", 1, IRET\n";
            current_time += 1;
        }
        else if(activity == "CPU"){
            execution += std::to_string(current_time) + ", " + std::to_string(duration_intr) +  ", CPU\n";
            current_time += duration_intr;
        }
        else if(activity == "END_IO"){
            execution += std::to_string(current_time) + ", " + std::to_string(end_io_delay) + ", end of I/O " + std::to_string(duration_intr) + ": interrupt\n";
            current_time += end_io_delay;
        }




        


        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
