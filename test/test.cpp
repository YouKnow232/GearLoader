#include "common/versionParsing.h"
#include "dependencyManager/configParser.h"
#include "dependencyManager/dependencyManager.h"
#include "gearLoaderApi/apiRegistry.h"
#include "gearLoaderApi/gearLoader_p.h"
#include "logger/logger.h"
#include "modFolderWalker/modFolderWalker.h"
#include "resourceRedirector/resourceRedirector.h"
#include <exception>
#include <filesystem>
#include <string>
#include <windows.h>

typedef void(*TestFunc)();

static std::string mainTestLogFileName = "./test/test.log";
inline Logger initTestLogger() {
    if (std::filesystem::exists(mainTestLogFileName)) {
        std::filesystem::remove(mainTestLogFileName);
    }
    return Logger(mainTestLogFileName, true);
}
static Logger _testLogger = initTestLogger();

inline void namedAssert(bool val, std::string errorMessage = "Unnamed assertion failed") {
    if (!val) throw std::runtime_error(errorMessage);
}

inline bool compareAsPath(std::string a, std::string b) {
    // std::filesystem::path pA(a);
    // std::filesystem::path pB(b);

    return std::filesystem::equivalent(a, b);
}

void testLogger() {
    std::string testLogFolder = "./test/testLogs";
    std::string testLogFileName = testLogFolder + "/test_nonverbose.log";
    std::string testLogVerboseFileName = testLogFolder + "/test_verbose.log";

    if (std::filesystem::exists(testLogFileName)) {
        std::filesystem::remove(testLogFileName);
    }
    if (std::filesystem::exists(testLogVerboseFileName)) {
        std::filesystem::remove(testLogVerboseFileName);
    }
    std::filesystem::path testLogPath = testLogFileName;
    std::filesystem::path testLogVerbosePath = testLogVerboseFileName;
    if (!std::filesystem::exists(testLogFolder)) {
        std::filesystem::create_directory(testLogFolder);
    }

    {
        Logger _testLog1(testLogFileName, false);
        _testLog1.log(VERBOSE, "Testing the debug Logger VERBOSE");
        _testLog1.log(DEBUG, "Testing the debug Logger DEBUG");
        _testLog1.log(INFO, "Testing the debug Logger INFO with args %d", 20);
        _testLog1.log(WARN, "Testing the debug Logger WARN");
        _testLog1.log(ERR, "Testing the debug Logger ERROR");

        Logger _testLog2(testLogVerboseFileName, true);
        _testLog2.log(VERBOSE, "Testing the debug Logger VERBOSE");
        _testLog2.log(DEBUG, "Testing the debug Logger DEBUG");
        _testLog2.log(INFO, "Testing the debug Logger INFO with args %d", 20);
        _testLog2.log(WARN, "Testing the debug Logger WARN");
        _testLog2.log(ERR, "Testing the debug Logger ERROR");
    }

    namedAssert(std::filesystem::exists(testLogFileName),
        "log file exists failed");
    namedAssert(std::filesystem::exists(testLogVerboseFileName),
        "verbose log file exists failed");
}

void testVersionParsing() {
    // ParseSemanticVersion Tests
    std::string test1 = "1.0.0";
    SemanticVersion expected1 {1,0,0};
    std::string test2 = "10.2.3015";
    SemanticVersion expected2 {10,2,3015};
    std::string test3 = "3";
    SemanticVersion expected3 {3,0,0};
    std::string test4 = "foobar";
    SemanticVersion defaultSemVer {0,0,0};

    SemanticVersion version = ParseSemanticVersion(test1);
    namedAssert(version == expected1, "parse 1.0.0");
    version = ParseSemanticVersion(test2);
    namedAssert(version == expected2, "parse 1.2.3");
    version = ParseSemanticVersion(test3);
    namedAssert(version == expected3, "parse 3.0.0");
    version = ParseSemanticVersion(test4);
    namedAssert(version == defaultSemVer, "parse foobar");


    // ParseVersionQualifier
    std::string qualifierTests[] = {
        ">=1.2.3", ">4.5.6", "=7.7.7", "<1.20.50", "<=6.99.125", "<=6"
    };
    SemanticVersion expectedVers[] = {
        {1,2,3}, {4,5,6}, {7,7,7}, {1,20,50}, {6,99,125}, {6,0,0}
    };
    Operator expectedOps[] = {
        Operator::EQ_OR_GREATER_THAN,
        Operator::GREATER_THAN,
        Operator::EQUAL,
        Operator::LESS_THAN,
        Operator::EQ_OR_LESS_THAN,
        Operator::EQ_OR_LESS_THAN,
    };

    SemanticVersion outVer;
    Operator outOp;
    bool success;

    for (int i = 0; i < size(qualifierTests); i++) {
        success = ParseVersionQualifier(qualifierTests[i], outOp, outVer);
        namedAssert(success, std::format("test {} failed to parse", i));
        namedAssert(outOp == expectedOps[i], std::format("test {} incorrectly parsed operator", i));
        namedAssert(outVer == expectedVers[i], std::format("test {} incorrectly parsed version", i));
    }
}


void testAPIRegistry() {
    APIRegistry _modReg;

    // Test data
    ModApi testApi1 = {
        name: "Test1",
        version: {1,0,0},
        api: reinterpret_cast<void*>(0x10)
    };
    ModApi testApi2Low = {
        name: "Test2",
        version: {1,1,0},
        api: reinterpret_cast<void*>(0x20)
    };
    ModApi testApi2High = {
        name: "Test2",
        version: {1,5,0},
        api: reinterpret_cast<void*>(0x30)
    };

    // Register test apis
    bool success = _modReg.put(testApi1.api, testApi1.name.c_str(), testApi1.version, &_testLogger);
    namedAssert(success, "put 1 failed");
    success = _modReg.put(testApi2Low.api, testApi2Low.name.c_str(), testApi2Low.version, &_testLogger);
    namedAssert(success, "put 2 low failed");
    bool duplicatePutReturn = _modReg.put(testApi1.api, testApi1.name.c_str(), testApi1.version, &_testLogger);
    namedAssert(!duplicatePutReturn, "Duplicate put didn't return false");
    success = _modReg.put(testApi2High.api, testApi2High.name.c_str(), testApi2High.version, &_testLogger);
    namedAssert(success, "put 2 high failed");

    // Retrieve
    ModApi retApi1;
    ModApi retApi2Low;
    ModApi retApi2High;
    ModApi fooApi;
    success = _modReg.get(testApi1.name.c_str(), testApi1.version, Operator::EQ_OR_GREATER_THAN, retApi1, &_testLogger);
    namedAssert(success, "Get 1 failed");
    namedAssert(retApi1.api == testApi1.api, "api retrieval 1 failed");
    namedAssert(retApi1.version == testApi1.version, "api version parse failed");

    success = _modReg.get(testApi2Low.name.c_str(), testApi2Low.version, Operator::EQUAL, retApi2Low, &_testLogger);
    namedAssert(success, "Get 2 low failed");
    namedAssert(retApi2Low.api == testApi2Low.api, "api retrieval 2 low failed");
    namedAssert(retApi2Low.version == testApi2Low.version, "api version 2 low failed");

    success = _modReg.get(testApi2High.name.c_str(), {0,0,0}, Operator::EQ_OR_GREATER_THAN, retApi2High, &_testLogger);
    namedAssert(success, "Get 2 high failed");
    namedAssert(retApi2High.api == testApi2High.api, "api retrieval 2 high failed");
    namedAssert(retApi2High.version == testApi2High.version, "api version 2 high failed");

    bool garbageGet = _modReg.get("Foobar", {0,0,0}, Operator::EQ_OR_GREATER_THAN, fooApi, &_testLogger);
    namedAssert(garbageGet == false, "Requesting unregistered API should've return false");
}

inline std::string toString(DependencyManifest& dm) {
    std::stringstream ss;
    ss << dm.name << " " << ToString(dm.versionOperator) << dm.version << ", optional:" << dm.optional;
    return ss.str();
}
void testConfigParser() {

    std::vector<DependencyManifest> expectedDeps = {
        { 
            name: "test-dependency-1",
            version: {1,0,0},
            versionOperator: Operator::EQ_OR_GREATER_THAN,
            optional: false,
        },
        {
            name: "test-dependency-2",
            version: {0,2,6},
            versionOperator: Operator::GREATER_THAN,
            optional: false,
        },
        {
            name: "test-dependency-3",
            version: {2,0,0},
            versionOperator: Operator::EQUAL,
            optional: true,
        },
        {
            name: "no-version-requirement",
            version: {0,0,0},
            versionOperator: Operator::EQ_OR_GREATER_THAN,
            optional: false,
        },
    };
    ModManifest expected = {
        name: "dependency-test-mod",
        version: {1,0,0},
        path: std::filesystem::current_path() / "test" / "customEntryPointPath" / "bar.dll",
        dependencies: expectedDeps
    };

    std::filesystem::path path = std::filesystem::current_path() / "test/testConfig.json";
    ModManifest manifest = ParseConfig(path);

    namedAssert(manifest.name.compare(expected.name) == 0,
        "parse mod name failed");
    namedAssert(manifest.version == expected.version,
        "parse version failed");
    namedAssert(manifest.path.compare(expected.path) == 0, "parse entryPoint path failed");
    namedAssert(manifest.dependencies.size() == expected.dependencies.size(),
        "Number of dependencies didn't match");
    _testLogger.log(INFO, "parsed: %s | expected: %s", toString(manifest.dependencies[0]).c_str(), toString(expected.dependencies[0]).c_str());
    namedAssert(manifest.dependencies[0] == expected.dependencies[0], "dep 1 mismatch");
    _testLogger.log(INFO, "parsed: %s | expected: %s", toString(manifest.dependencies[1]).c_str(), toString(expected.dependencies[1]).c_str());
    namedAssert(manifest.dependencies[1] == expected.dependencies[1], "dep 2 mismatch");
    _testLogger.log(INFO, "parsed: %s | expected: %s", toString(manifest.dependencies[2]).c_str(), toString(expected.dependencies[2]).c_str());
    namedAssert(manifest.dependencies[2] == expected.dependencies[2], "dep 3 mismatch");
    _testLogger.log(INFO, "parsed: %s | expected: %s", toString(manifest.dependencies[3]).c_str(), toString(expected.dependencies[3]).c_str());
    namedAssert(manifest.dependencies[3] == expected.dependencies[3], "dep 4 mismatch");
    namedAssert(manifest.dependencies[0].name.compare(expected.dependencies[0].name) == 0,
        "parse first dependency name failed");
    namedAssert(manifest.dependencies[1].name.compare(expected.dependencies[1].name) == 0,
        "parse second dependency name failed");
    namedAssert(manifest.dependencies[1].version == expected.dependencies[1].version,
        "parse second dependency version failed");

    std::filesystem::path modAPath = std::filesystem::current_path() / "test/modFolderTest/modA/config.json";
    ModManifest manifest2 = ParseConfig(modAPath);
    namedAssert(manifest2.name.compare("modA") == 0,
        "parse modA name failed");
    namedAssert(manifest2.path.filename().compare("dummy.dll") == 0,
        "incorrect dll path");


    // entry test
    std::filesystem::path entryTestPath = std::filesystem::current_path() / "test/modFolderTest/modB/config.json";
    ModManifest modBManifest = ParseConfig(entryTestPath);

    namedAssert(std::filesystem::exists(modBManifest.path) &&
        !std::filesystem::is_directory(modBManifest.path),
        "Parsed entry path does not exists or is not a file");
}

inline bool searchFile(std::string filePath, std::string str) {
    std::ifstream stream;
    stream.open(filePath);

    std::string line;
    while(std::getline(stream, line, '\n')) {
        if (line.find(str) != std::string::npos) {
            return true;
        }
    }

    return false;
}

inline bool contains(std::vector<ModManifest>& vector, ModManifest& element) {
    return std::find_if(
        vector.begin(),
        vector.end(),
        [&element](const ModManifest &arg) { return arg.name == element.name; }
    ) != vector.end();
}
void testDependencyManager() {
    std::string depManLogFile = "./test/testLogs/DependencyManagerTest1.log";
    if (std::filesystem::exists(depManLogFile)) {
        std::filesystem::remove(depManLogFile);
    }

    Logger log(depManLogFile, true);

    // prep test data
    std::vector<DependencyManifest> depListA = {
        {"modB", {1,1,0}, Operator::EQUAL, false},
        {"modC", {1,0,0}, Operator::EQ_OR_GREATER_THAN, false}
    };
    ModManifest modA = {
        name: "modA",
        version: {1,0,0},
        modLoaderVersion: GEARLOADER_VERSION_SEM_VER,
        path: "./A",
        dependencies: depListA};
    ModManifest modB = {
        name: "modB",
        version: {1,1,0},
        modLoaderVersion: GEARLOADER_VERSION_SEM_VER,
        path: "./B"};
    ModManifest modC = {
        name: "modC",
        version: {1,0,0},
        modLoaderVersion: GEARLOADER_VERSION_SEM_VER,
        path: "./C"};

    std::vector<DependencyManifest> depListD = {
        {"modB", {1,0,0}, Operator::LESS_THAN, false}
    };
    ModManifest modD = {
        name: "modD",
        version: {1,0,0},
        modLoaderVersion: GEARLOADER_VERSION_SEM_VER,
        path: "./D",
        dependencies: depListD};
    std::vector<DependencyManifest> depListE = {
        {"modC", {0,4,0}, Operator::LESS_THAN, true}
    };
    ModManifest modE = {
        name: "modE",
        version: {1,0,0},
        modLoaderVersion: GEARLOADER_VERSION_SEM_VER,
        path: "./E",
        dependencies: depListE};
    std::vector<DependencyManifest> depListF = {
        {"Foobar", {1,0,0}, Operator::EQUAL, false}
    };
    ModManifest modF = {
        name: "modF",
        version: {1,0,0},
        modLoaderVersion: GEARLOADER_VERSION_SEM_VER,
        path: "./F",
        dependencies: depListF};
    std::vector<DependencyManifest> depListG = {
        {"modB", {1,0,0}, Operator::EQ_OR_GREATER_THAN, false}
    };
    ModManifest modG = {
        name: "modG",
        version: {1,0,0},
        modLoaderVersion: GEARLOADER_VERSION_SEM_VER,
        path: "./G",
        dependencies: depListG};

    DependencyManager depMan;

    depMan.registerManifest(modA);
    depMan.registerManifest(modB);
    depMan.registerManifest(modC);
    depMan.registerManifest(modD);  // fail (failed version constraint)
    depMan.registerManifest(modE);
    depMan.registerManifest(modF);  // fail (missing dep)
    depMan.registerManifest(modG);  // succeeds (newer dep version present)
    depMan.finalize(log);

    auto loadOrder = depMan.createLoadOrderVector();

    std::string graphStr = depMan.printGraph();
    log.log(DEBUG, ("mod load order:\n" + graphStr).c_str());

    namedAssert(loadOrder.size() == 5,
        "5 mods should have been loaded");
    namedAssert(!contains(loadOrder, modD),
        "modD should fail to load due to dep constraint");
    namedAssert(contains(loadOrder, modE),
        "modE should be loaded despite failed dep constraint due to optional flag");
    namedAssert(!contains(loadOrder, modF),
        "modF should fail to load due to missing dependency");
    namedAssert(contains(loadOrder, modG),
        "modG should be loaded if newer version exists");
}

void testDependencyManagerCycles() {
    std::string depManLogFile = "./test/testLogs/DependencyManagerTest2.log";
    if (std::filesystem::exists(depManLogFile)) {
        std::filesystem::remove(depManLogFile);
    }
    {
        Logger log(depManLogFile, true);

        // prep test data
        std::vector<DependencyManifest> depListA = {
            DependencyManifest{"modB", SemanticVersion{1,0,0}},
        };
        std::vector<DependencyManifest> depListB = {
            DependencyManifest{"modC", SemanticVersion{1,0,0}},
        };
        std::vector<DependencyManifest> depListC = {
            DependencyManifest{"modA", SemanticVersion{1,0,0}},
        };
        ModManifest modA = ModManifest{
            name: "modA",
            version: {1,0,0},
            modLoaderVersion: GEARLOADER_VERSION_SEM_VER,
            path: "./fakeA",
            dependencies: depListA
        };
        ModManifest modB = ModManifest{
            name: "modB",
            version: {1,0,0},
            modLoaderVersion: GEARLOADER_VERSION_SEM_VER,
            path: "./fakeB",
            dependencies: depListB
        };
        ModManifest modC = ModManifest{
            name: "modC",
            version: {1,0,0},
            modLoaderVersion: GEARLOADER_VERSION_SEM_VER,
            path: "./fakeC",
            dependencies: depListC
        };
        ModManifest modD = ModManifest{
            name: "modD",
            version: {1,2,3},
            modLoaderVersion: GEARLOADER_VERSION_SEM_VER,
            path: "./fakeD"
        };

        DependencyManager depMan;

        depMan.registerManifest(modA);
        depMan.registerManifest(modB);
        depMan.registerManifest(modC);
        depMan.registerManifest(modD);
        depMan.finalize(log);

        std::string graphStr = depMan.printGraph();
        log.log(DEBUG, ("mod load order:\n" + graphStr).c_str());

        namedAssert(depMan.createLoadOrderVector().size() == 1,
            "load order size wasn't 1");
    }
    // Wrap logger in code block to deconstruct it before searching log file
    // Something up with file lock here
    // namedAssert(searchFile(depManLogFile, "ERROR"),
    //     "Errors from logger");
}

void testDependencyManagerIndependentModErrors() {
    // prep test data
    ModManifest control = ModManifest{
        name: "modA",
        version: {1,0,0},
        modLoaderVersion: GEARLOADER_VERSION_SEM_VER,
        path: "./fakeA"
    };
    ModManifest requiresFutureGearLoaderVer = ModManifest{
        name: "modB",
        version: {1,0,0},
        modLoaderVersion: {999,999,999},    // Future version requirement
        path: "./fakeB"
    };
    ModManifest ignoredMod = ModManifest{
        name: "modC",
        version: {1,0,0},
        modLoaderVersion: GEARLOADER_VERSION_SEM_VER,    // Future version requirement
        ignore: true,
        path: "./fakeC"
    };

    DependencyManager depMan;

    depMan.registerManifest(control);
    depMan.registerManifest(requiresFutureGearLoaderVer);
    depMan.registerManifest(ignoredMod);
    depMan.finalize(_testLogger);

    namedAssert(depMan.createLoadOrderVector().size() == 1,
        "load order size wasn't 1");
}

void testModFolderWalker() {
    std::filesystem::path curDirectory = std::filesystem::current_path();
    std::filesystem::path testModFolder = std::filesystem::current_path() / "test\\modFolderTest";

    WalkModFolder(
        testModFolder,
        [](std::filesystem::directory_entry modFolder, std::filesystem::directory_entry file) {
            // intentionally blank
        },
        _testLogger);
}

void testResourceRedirector() {
    GLLogger* noLogger = nullptr;
    RegisterModResources("./test/modFolderTest", noLogger);

    const char* test1 = "test1.bin";
    const char* test2 = "test2.bin";
    const char* test3 = "testDir/subDirTest.bin";
    const char* test4 = "foo.bar";

    const char* result1 = GetResourceOverride(test1, noLogger);
    const char* result2 = GetResourceOverride(test2, noLogger);
    const char* result3 = GetResourceOverride(test3, noLogger);
    const char* result4 = GetResourceOverride(test4, noLogger);
    
    const char* expected1 = "./test/modFolderTest/modA/Resource/test1.bin";
    const char* expected2 = "./test/modFolderTest/modB/Resource/test2.bin";
    const char* expected3 = "./test/modFolderTest/modA/Resource/testDir/subDirTest.bin";
    const char* expected4 = "";

    std::ostringstream ss1;
    ss1 << "basic redirect: " << result1 << " == " << expected1;
    namedAssert(compareAsPath(result1, expected1), ss1.str());

    std::ostringstream ss2;
    ss2 << "two redirect: " << result2 << " == " << expected2;
    namedAssert(compareAsPath(result2, expected2), ss2.str());
    
    std::ostringstream ss3;
    ss3 << "sub directory redirect: " << result3 << " == " << expected3;
    namedAssert(compareAsPath(result3, expected3), ss3.str());
    
    std::ostringstream ss4;
    ss4 << "no redirect: result was not the empty string";
    namedAssert(*result4 == 0, ss4.str());
}

inline bool test(std::string testName, TestFunc testFunc) {
    std::cout << testName;

    bool result;
    try {
        testFunc();
        std::cout << "pass";
        result = true;
    }
    catch (std::runtime_error e) {
        std::cout << "fail\t" << e.what();
        result = false;
    }
    
    std::cout << std::endl;
    return result;
}

int main() {
    struct NamedTest {
        std::string Name;
        TestFunc Test;
    };
    std::vector<NamedTest> tests = {
        {"Logger tests               ", testLogger},
        {"Version parsing tests      ", testVersionParsing},
        {"API Registry tests         ", testAPIRegistry},
        {"Config parsing tests       ", testConfigParser},
        {"Dependency manager tests   ", testDependencyManager},
        {" - Circular Dep test       ", testDependencyManagerCycles},
        {" - Required Ver test       ", testDependencyManagerIndependentModErrors},
        {"Mod Folder Walker test     ", testModFolderWalker},
        {"Test Resource Redirector   ", testResourceRedirector},
    };

    int total = tests.size();
    int passes = 0;

    for (auto& namedTest : tests) {
        if (test(namedTest.Name, namedTest.Test)) passes++;
    }

    std::cout << std::endl <<
        "Results: " << passes << "/" << total << std::endl <<
        "See " << mainTestLogFileName << " for more information." << std::endl;

    return total - passes;
}
