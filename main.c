
#include "Tests/CommunicationManagerTests/CommunicationManager.h"
#include "Tests/Uart/uartTest.h"
#include "Tests/Energy_Logger/Logger_Test.h"
int main(void)
{
    CommManagerTestsMain();
    LoggerTestMain();

    //UartTestMain();
    while(1)
    {
        
    }
     
}

