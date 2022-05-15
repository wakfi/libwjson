// Standard library modules
#include <string>
#include <iostream>
#include <fstream>

// libwjson modules
#include <core/token.cpp>
#include <core/json_exception.cpp>
#include <core/lexer.cpp>
#include <core/parser.cpp>
#include <core/ast.cpp>

// GTest
#include <gtest/gtest.h>

//----------------------------------------------------------------------
// Constants
//----------------------------------------------------------------------

#define TEST_FILES_DIR_PATH input_files
#define TEST_FILE(file_name) GTEST_STRINGIFY_(TEST_FILES_DIR_PATH/file_name)
#define TEST_SPEC_FILE(file_name) GTEST_STRINGIFY_(TEST_FILES_DIR_PATH/rfc8259_examples/file_name)

//----------------------------------------------------------------------
// libwjson/core Tests
//----------------------------------------------------------------------

using namespace std;

TEST(WJSON_CORE, Literal) {
    ifstream input_stream(TEST_SPEC_FILE(literal.json));
    EXPECT_EQ(false, input_stream.fail());

    try {
        // create the lexer & parser
        Lexer lexer(input_stream);
        Parser parser(lexer);
        JSONDocument ast_root_node;
        parser.parse(ast_root_node);
        EXPECT_STRCASEEQ("true", ast_root_node.root->first_token().lexeme().c_str());
    } catch (JSONException e) {
        cerr << e.to_string() << endl;
        throw e;
    }
}

// Main
int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
