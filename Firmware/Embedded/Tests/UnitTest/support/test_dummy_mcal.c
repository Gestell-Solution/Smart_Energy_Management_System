/**
 * @file test_template.c
 * @brief Unit test template for {{PROJECT_NAME}}
 * 
 * This is a template for creating unit tests using the Unity framework.
 * Copy this file and customize for your module.
 * 
 * @author {{AUTHOR_NAME}}
 * @date {{DATE}}
 */

/*==============================================================================
 * INCLUDES
 *============================================================================*/
#include "unity.h"
#include "module_under_test.h"  // Replace with actual module

/*==============================================================================
 * TEST SETUP AND TEARDOWN
 *============================================================================*/

/**
 * @brief Setup function - runs before EACH test
 * 
 * Initialize test environment, reset static variables, etc.
 */
void setUp(void)
{
    // Initialize module or reset state
    // Example: Module_Init();
}

/**
 * @brief Teardown function - runs after EACH test
 * 
 * Clean up test environment, free resources, etc.
 */
void tearDown(void)
{
    // Clean up
    // Example: Module_DeInit();
}

/*==============================================================================
 * TEST CASES
 *============================================================================*/

/**
 * @brief Test that module initializes correctly
 * 
 * Test naming convention:
 * test_<ModuleName>_<FunctionName>_Should<ExpectedBehavior>_When<Condition>
 */
void test_Module_Init_ShouldReturnOK_WhenCalledFirstTime(void)
{
    // Arrange (setup test data)
    StdReturn_t result;
    
    // Act (call function under test)
    result = Module_Init();
    
    // Assert (verify expected behavior)
    TEST_ASSERT_EQUAL(STD_OK, result);
}

/**
 * @brief Test that function handles NULL pointer
 */
void test_Module_Function_ShouldReturnError_WhenGivenNullPointer(void)
{
    // Arrange
    StdReturn_t result;
    
    // Act
    result = Module_Function(NULL);
    
    // Assert
    TEST_ASSERT_EQUAL(STD_INVALID_PARAM, result);
}

/**
 * @brief Test that function processes data correctly
 */
void test_Module_ProcessData_ShouldProcessCorrectly_WhenGivenValidInput(void)
{
    // Arrange
    uint8_t inputData = 0x55;
    uint8_t expectedOutput = 0xAA;
    uint8_t actualOutput;
    
    // Act
    actualOutput = Module_ProcessData(inputData);
    
    // Assert
    TEST_ASSERT_EQUAL_HEX8(expectedOutput, actualOutput);
}

/**
 * @brief Test boundary conditions
 */
void test_Module_Function_ShouldHandleBoundary_WhenGivenMaxValue(void)
{
    // Arrange
    uint8_t maxValue = 0xFF;
    StdReturn_t result;
    
    // Act
    result = Module_Function(maxValue);
    
    // Assert
    TEST_ASSERT_EQUAL(STD_OK, result);
}

/**
 * @brief Test state machine transitions (if applicable)
 */
void test_Module_StateMachine_ShouldTransition_WhenEventOccurs(void)
{
    // Arrange
    Module_Init();
    ModuleState_t expectedState = STATE_RUNNING;
    
    // Act
    Module_ProcessEvent(EVENT_START);
    ModuleState_t actualState = Module_GetState();
    
    // Assert
    TEST_ASSERT_EQUAL(expectedState, actualState);
}

/*==============================================================================
 * MAIN TEST RUNNER
 *============================================================================*/

/**
 * @brief Main function - runs all tests
 * 
 * @return int Test result (0 = all pass, non-zero = failures)
 */
int main(void)
{
    UNITY_BEGIN();
    
    // Run all test cases
    RUN_TEST(test_Module_Init_ShouldReturnOK_WhenCalledFirstTime);
    RUN_TEST(test_Module_Function_ShouldReturnError_WhenGivenNullPointer);
    RUN_TEST(test_Module_ProcessData_ShouldProcessCorrectly_WhenGivenValidInput);
    RUN_TEST(test_Module_Function_ShouldHandleBoundary_WhenGivenMaxValue);
    RUN_TEST(test_Module_StateMachine_ShouldTransition_WhenEventOccurs);
    
    return UNITY_END();
}

/*==============================================================================
 * TEST HELPERS (Optional)
 *============================================================================*/

/**
 * @brief Helper function to create test data
 */
static void CreateTestData(TestData_t *data)
{
    // Initialize test data structure
    data->field1 = 0;
    data->field2 = 100;
}

/**
 * @brief Helper to verify complex state
 */
static void VerifySystemState(void)
{
    // Check multiple conditions
    TEST_ASSERT_TRUE(Module_IsInitialized());
    TEST_ASSERT_EQUAL(0, Module_GetErrorCount());
}
