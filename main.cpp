#include <systemc.h>
#include <iostream>
#include <string>
#include <limits>

#define FIFO_SIZE 7


class HifiInterface : public sc_interface {
public:
    virtual void write(int data) = 0;
    virtual int read() = 0;
    virtual bool is_fifo_not_empty() = 0;
    virtual void clear_fifo() = 0;
    virtual void errorHandle() = 0;
};

class HifiControl : public HifiInterface, public sc_prim_channel {
public:
    HifiControl(sc_module_name name) : sc_prim_channel(name), 
    fifo(FIFO_SIZE) {}

    void write(int data) override {
        fifo.write(data);
    }

    int read() override {
        return fifo.read();
    }

    bool is_fifo_not_empty() override {
        return fifo.num_available() > 0;
    }

    void clear_fifo() override {
        while (fifo.num_available() > 0) {
            fifo.read();
        }
    }

    void errorHandle() override {
        if(is_fifo_not_empty()) {
            unsigned int value = fifo.read();
            bool valid = false;
            for(const int validValue : {1,2,3,4,5,6,7,100,101,102}) {
                if(value == validValue) {
                    valid = true;
                    break;
                }
            }
            
            if(!valid) {
                std::cout << "Invalid input detected. System stopping." << std::endl;
                clear_fifo();
                sc_stop();
            }
        }
    }

private:
    sc_fifo<unsigned int> fifo;
};



SC_MODULE(SubModule){
    sc_port<HifiInterface> port;
    sc_event& evFromModule1;
    sc_event& evToModule1;

    SC_HAS_PROCESS(SubModule);
    SubModule(sc_module_name name, sc_event& evFrom, sc_event& evTo) : sc_module(name), evFromModule1(evFrom), evToModule1(evTo) {
        SC_THREAD(volume_thread);
    }
        void volume_thread() {
        while(true) {
            wait(evFromModule1);            
            if(port->is_fifo_not_empty() && port->read() == 7) {
                std::cout << "Enter volume level (0-100): ";
                int volume;
                std::cin>>volume;
                std::cout << "Volume set to: " << volume << std::endl;
            } 
            wait(50, SC_MS);
            std::cout << " x";
            evToModule1.notify();
            std::cout << "d";
        }
    }


};

SC_MODULE(Processor1) {
    sc_port<HifiInterface> port;
    sc_event e1, e2, e3, e4, e5, e6, e7, eventMode;
    SubModule* sub_module = new SubModule("Sub_Module", e7, eventMode);
    sc_event evToModule2;
    sc_event* evFromModule2;
    

    SC_CTOR(Processor1) {
        SC_THREAD(set_mode);
        SC_THREAD(radio_thread);
        SC_THREAD(cassette_thread);
        SC_THREAD(turntable_thread);
        SC_THREAD(cd_thread);
        SC_THREAD(dvd_thread);
        SC_THREAD(aux_thread);
        sub_module->port(port);
    }

    bool if_error(int mode){
        return mode < 1 || mode > 7;
    }
  
    void set_mode() {
        while(true) {
            std::cout << "\nSelect mode:\n"
                      << "1. Radio\n"
                      << "2. Cassette\n"
                      << "3. Turntable\n"
                      << "4. CD\n"
                      << "5. DVD\n"
                      << "6. AUX\n"
                      << "7. volume\n";
            int mode=0;
            std::cin>>mode;
            port->write(mode);
            if(if_error(mode)){
                std::cout<<"nie";
                continue;
            }

            wait(50, SC_MS);
            switch (mode) {
            case 1:
                e1.notify();
                break;
            case 2:
                e2.notify();
                break;
            case 3:
                e3.notify();
                break;
            case 4:
                e4.notify();
                break;
            case 5:
                e5.notify();
                break;
            case 6:
                e6.notify();
                break;
            case 7:
                e7.notify();
                break;
            default:
                std::cout << "Unexpected mode value. No action taken.\n";
        }
            wait(eventMode);
        }
    }

    void radio_thread() {
        while(true) {
            wait(e1);
            if(port->is_fifo_not_empty()) {
                int input = port->read();
                if(input == 1) {
                    std::cout << "Radio Mode Active\n";
                    std::cout << "   .-------------------------.\n";
                    std::cout << "  |  .-------------------.  |\n";
                    std::cout << "  |  |                   |  |\n";
                    std::cout << "  |  |   88.5 MHz FM     |  |\n";
                    std::cout << "  |  |   [O] Playing     |  |\n";
                    std::cout << "  |  '-------------------'  |\n";
                    std::cout << "  |   .    .    .    .      |\n";
                    std::cout << "  |  [|]  [|]  [|]  [|]     |\n";
                    std::cout << "  |      O            O     |\n";
                    std::cout << "  '-------------------------'\n\n";

                    std::cout<< "Select wave type:\n1. AM\n2. FM\n";
                    
                    int waveType;
                    std::cin>>waveType;
                    std::cout << "Radio " << (waveType == 1 ? "AM" : "FM") << " selected\n";
                }
            }
            wait(50, SC_MS);
            eventMode.notify();
        }
    }

    void cassette_thread() {
        while(true) {
            wait(e2);
            if(port->is_fifo_not_empty() && port->read() == 2) {
                std::cout << "Cassette Mode Active\n";
                std::cout << "  .----------------.  \n";
                std::cout << " / .--------------. \\ \n";
                std::cout << "| |  _________    | |\n";
                std::cout << "| | |_   ___  |   | |\n";
                std::cout << "| |   | |_  \\_|   | |\n";
                std::cout << "| |   |  _|       | |\n";
                std::cout << "| |  _| |_        | |\n";
                std::cout << "| | |_____|       | |\n";
                std::cout << "| |                | |\n";
                std::cout << " \\ '--------------' /  \n";
                std::cout << "  '----------------'   \n";
            }
            wait(50, SC_MS);
            eventMode.notify();
        }
    }

    void turntable_thread() {
        while(true) {
            wait(e3);
            if(port->is_fifo_not_empty() && port->read() == 3) {
                std::cout << "Turntable Mode Active\n";
                std::cout << "      _______\n";
                std::cout << "     |       |\n";
                std::cout << "     |       |\n";
                std::cout << "   --|-------|--\n";
                std::cout << "  |  |       |  |\n";
                std::cout << " /   |_______|   \\ \n";
                std::cout << "/_________________\\ \n";
                std::cout << "|                 |\n";
                std::cout << "|     O     O     |\n";
                std::cout << "\\_________________/ \n\n";
            }
            wait(50, SC_MS);
            eventMode.notify();
        }
    }

    void cd_thread() {
        while(true) {
            wait(e4);
            if(port->is_fifo_not_empty() && port->read() == 4) {
                std::cout<<std::endl << "CD Mode Active\n";
                std::cout << "       _________       \n";
                std::cout << "    .-'         '-.    \n";
                std::cout << "  .'    .-----.    '.  \n";
                std::cout << " /     /       \\     \\ \n";
                std::cout << ":     :         :     :\n";
                std::cout << "|     |    O    |     |\n";
                std::cout << ":     :         :     :\n";
                std::cout << " \\     \\       /     / \n";
                std::cout << "  '.    '-----'    .'  \n";
                std::cout << "    '-._________.-'    \n\n";
                std::cout << "Select disc type:\n";
                std::cout << "0: CD Audio\n1: CD MP3\n2: DVD Audio\n3: DVD MP3\n";
                
                int discType;
                std::cin>>discType;
                switch(discType) {
                    case 0:
                        std::cout << "CD Audio\n";
                        break;
                    case 1:
                        std::cout << "CD MP3\n";
                        break;
                    default:
                        std::cout << (discType == 2 ? "DVD Audio\n" : "DVD MP3\n");
                        break;
                }
                evToModule2.notify(SC_ZERO_TIME);
                wait(*evFromModule2);
            }
            wait(50, SC_MS);
            eventMode.notify();
        }
    }

    void dvd_thread() {
        while(true) {
            wait(e5);
            if(port->is_fifo_not_empty() && port->read() == 5) {
                std::cout << "DVD Mode Active\n";
                std::cout << "  ___________\n";
                std::cout << " |           |\n";
                std::cout << " |   DVD     |\n";
                std::cout << " |           |\n";
                std::cout << " |___________|\n";
                std::cout << "    ||   ||\n";
                std::cout << "    ||   ||\n";
                std::cout << "    ||   ||\n\n";
            }
            wait(50, SC_MS);
            eventMode.notify();
        }
    }

    void aux_thread() {
        while(true) {
            wait(e6);
            if(port->is_fifo_not_empty() && port->read() == 6) {
                std::cout << "AUX Mode Active\n";
                std::cout << "  /===========\\\n";
                std::cout << " |    AUX      |\n";
                std::cout << " |   -------   |\n";
                std::cout << " |_____________|\n";
                std::cout << "   |||||||||||\n";
                std::cout << "   |||||||||||\n";
                std::cout << "   |||||||||||\n\n";
            }
            wait(50, SC_MS);
            eventMode.notify();
        }
    }

    void set_event_from_module2(sc_event* ev) {
    if (ev == nullptr) {
        std::cout << "Error: Null event pointer\n";
        sc_stop();
        return;
    }
    evFromModule2 = ev;
}

    void bind_ports(HifiInterface &comm) {
        port(comm);
    }
};


SC_MODULE(Processor2) {
    sc_port<HifiInterface> port;
    sc_event modeEv, p1, p2, p3, p4;
    sc_event evToModule1;
    sc_event* evFromModule1;

    SC_CTOR(Processor2){
        SC_THREAD(disc_controller);
        SC_THREAD(play_disc);
        SC_THREAD(stop_disc);
        SC_THREAD(next_song);
        SC_THREAD(prev_song);
    }

    void disc_controller() {
        while(true) {
            wait(*evFromModule1);
            std::cout << "\nCD Controls:\n"
                  << "1. Play\n"
                  << "2. Stop\n"
                  << "3. Next Track\n"
                  << "4. Previous Track\n";
            int data=0;
            std::cin>>data;
            if(if_error(data)){
                std::cout<<"nie";
                port->clear_fifo();
            }
            port->write(data);
            wait(50, SC_MS);
            switch (data) {
            case 1:
                p1.notify();
                break;
            case 2:
                p2.notify();
                break;
            case 3:
                p3.notify();
                break;
            case 4:
                p4.notify();
                break;
            }
            wait(modeEv);
        }
    }

    void play_disc() {
        while(true) {
            wait(p1);
            if(port->is_fifo_not_empty() && port->read() == 1) {
                std::cout << "Playing\n";
            }
            evToModule1.notify(10, SC_MS);
            wait(*evFromModule1);
            wait(50, SC_MS);
            modeEv.notify();
        }
    }

    void stop_disc() {
        while(true) {
            wait(p2);
            if(port->is_fifo_not_empty() && port->read() == 2) {
                std::cout << "Stopped\n";
            }
            evToModule1.notify(10, SC_MS);
            wait(*evFromModule1);
            wait(50, SC_MS);
            modeEv.notify();
        }
    }

    void next_song() {
        while(true) {
            wait(p3);
            if(port->is_fifo_not_empty() && port->read() == 3) {
                std::cout << "Next Track\n";
            }
            evToModule1.notify(10, SC_MS);
            wait(*evFromModule1);
            wait(50, SC_MS);
            modeEv.notify();
        }
    }

    void prev_song() {
        while(true) {
            wait(p4);
            if(port->is_fifo_not_empty() && port->read() == 4) {
                std::cout << "Previous Track\n";
            }
            evToModule1.notify(10, SC_MS);
            wait(*evFromModule1);
            wait(50, SC_MS);
            modeEv.notify();
        }
    }

    void bind_ports(HifiInterface &comm) {
        port.bind(comm);
    }

    bool if_error(int input) {
        return input < 1 || input > 4; 
    }

    void set_event_from_module1(sc_event* ev) {
        if (ev == nullptr) {
            std::cout << "Error: Null event pointer\n";
            sc_stop();
            return;
        }
        evFromModule1 = ev;
    }
};



int sc_main(int argc, char* argv[]) {
    HifiControl control("control");
    Processor1 mode_selector("ModeSelector");
    Processor2 disc_handler("DiscHandler");

    mode_selector.bind_ports(control);
    disc_handler.bind_ports(control);

    mode_selector.set_event_from_module2(&disc_handler.evToModule1);
    disc_handler.set_event_from_module1(&mode_selector.evToModule2);
    
    sc_start();
    return 0;
}