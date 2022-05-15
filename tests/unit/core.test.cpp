// Standard library modules
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

// libwjson modules
#include <core/token.cpp>
#include <core/json_exception.cpp>
#include <core/lexer.cpp>
#include <core/parser.cpp>
#include <core/ast.cpp>

// GTest
#include <gtest/gtest.h>

// wjsoncompact
#include <wjsoncompact/wjsoncompact.cpp>

//----------------------------------------------------------------------
// Constants & Macros
//----------------------------------------------------------------------

#define TEST_FILES_DIR_PATH input_files
#define TEST_FILE(file_name) GTEST_STRINGIFY_(TEST_FILES_DIR_PATH/file_name)
#define TEST_SPEC_FILE(file_name) GTEST_STRINGIFY_(TEST_FILES_DIR_PATH/rfc8259_examples/file_name)
#define INPUT_SPEC(file_name) ifstream input(TEST_SPEC_FILE(file_name));\
EXPECT_EQ(false, input.fail())
#define INPUT(file_name) ifstream input(TEST_FILE(file_name));\
EXPECT_EQ(false, input.fail())

//----------------------------------------------------------------------
// Test Body Macros
//----------------------------------------------------------------------

#define TEST_AGAINST_FIRST_TOKEN(expectedValue) try {\
        Lexer lexer(input);\
        Parser parser(lexer);\
        JSONDocument ast_root_node;\
        parser.parse(ast_root_node);\
        EXPECT_STRCASEEQ(expectedValue, ast_root_node.root->first_token().lexeme().c_str());\
    } catch (JSONException e) {\
        cerr << e.to_string() << endl;\
        throw e;\
    }

// We use the printer from wjsoncompact to create a minimilist representation of the AST.
// Be mindful that this does introduce an additional mode of failure
#define TEST_AGAINST_PRINTER(expectedValue) string rawJson = expectedValue;\
    ostringstream printerOut (ostringstream::ate);\
\
    try {\
        Lexer lexer(input);\
        Parser parser(lexer);\
        JSONDocument ast_root_node;\
        parser.parse(ast_root_node);\
        Printer printer(printerOut);\
        ast_root_node.accept(printer);\
\
        string parsedJson = printerOut.str();\
        EXPECT_STRCASEEQ(rawJson.c_str(), parsedJson.c_str());\
    } catch (JSONException e) {\
        cerr << e.to_string() << endl;\
        throw e;\
    }

//----------------------------------------------------------------------
// libwjson/core Tests
//----------------------------------------------------------------------

using namespace std;

// TEST SUITES
// WJSON_CORE_SPEC - Tests against example JSON samples found in RFC8259
// WJSON_CORE - Tests around the core modules

TEST(WJSON_CORE_SPEC, Literal) {
    INPUT_SPEC(literal.json);
    TEST_AGAINST_FIRST_TOKEN("true");
}

TEST(WJSON_CORE_SPEC, Number) {
    INPUT_SPEC(number.json);
    TEST_AGAINST_FIRST_TOKEN("42");
}

TEST(WJSON_CORE_SPEC, String) {
    INPUT_SPEC(string.json);
    // The quotation marks are not included in the stored lexeme
    TEST_AGAINST_FIRST_TOKEN("Hello World!");
}

TEST(WJSON_CORE_SPEC, Object) {
    INPUT_SPEC(rfc8259_obj_ex.json);
    TEST_AGAINST_PRINTER("{\"Image\":{\"Width\":800,\"Height\":600,\"Title\":\"View from 15th Floor\",\"Thumbnail\":{\"Url\":\"http://www.example.com/image/481989943\",\"Height\":125,\"Width\":100},\"Animated\":false,\"IDs\":[116,943,234,38793]}}");
}

TEST(WJSON_CORE_SPEC, Array) {
    INPUT_SPEC(rfc8259_arr_ex.json);
    TEST_AGAINST_PRINTER("[{\"precision\":\"zip\",\"Latitude\":37.7668,\"Longitude\":-122.3959,\"Address\":\"\",\"City\":\"SAN FRANCISCO\",\"State\":\"CA\",\"Zip\":\"94107\",\"Country\":\"US\"},{\"precision\":\"zip\",\"Latitude\":37.371991,\"Longitude\":-122.026020,\"Address\":\"\",\"City\":\"SUNNYVALE\",\"State\":\"CA\",\"Zip\":\"94085\",\"Country\":\"US\"}]");
}



TEST(WJSON_CORE, NumbersMisc) {
    // numbers.json has a JSON object with an assortment of valid JSON number values showcasing
    // a range of lexical features (e.g. normal positive numbers, decimals, negative sign,
    // scientific notation, etc)
    INPUT(numbers.json);
    TEST_AGAINST_PRINTER("{\"int\":3,\"another int\":103,\"negative\":-20,\"decimal\":1.3,\"leading 0 decimal\":0.5,\"negative decimal\":-51.92,\"negative leading 0 decimal\":-0.336,\"exp\":1e4,\"big exp\":2e21,\"bigger exp\":11e3,\"negative exp\":-4e5,\"exp with plus\":81e+1,\"exp with 0\":51e0,\"exp with minus\":37e-3,\"negative exp with plus\":-81e+1,\"negative exp with minus\":-23e-6,\"exp with frac and exp\":51.53e29}");
}



// Main
int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
