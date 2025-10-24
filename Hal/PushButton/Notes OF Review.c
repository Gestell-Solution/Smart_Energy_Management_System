// 1-Remove the inclusion of every  Libraries in the BTN_Private file  
// 2-Remove the extern uint8_t BTN_SystemMode; because it is not neccssary in the private file it self
// 3- The GIE Enable is not included to the files 
// Try to revise The doxygen Comments again ,I am not talking about making modules of your code like input parts But I find there is errors in
//writing the Comments and lastly just try to read you comments to know if you understand and keep going
// in the ISR action you are using the If Condition in toggling the value but there is better way 
// Val ^= 1; this will toggle the Value directly 
// For more serach on XOR 
