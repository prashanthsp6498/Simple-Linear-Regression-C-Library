const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    // Create a static library to collect all .o files
    const regressionCLib = b.addStaticLibrary(.{
        .name = "LinearRegression_C_Lib ",
        .target = target,
        .optimize = optimize,
    });

    const c_sources: [4][]const u8 = .{
        "./EDA/DataAnalysis.c",
        "./Regression/Linear.c",
        "./Regression/memory_deallocation.c",
        "./Regression/model_performance_with_regularization.c"
    };

    regressionCLib.linkLibC();
    for (c_sources) |c_file| {
        regressionCLib.addCSourceFile(.{
            .file = b.path(c_file),
            .flags = &.{
                "-g",
                "-Wall",
                "-Wextra",
                "-Werror",
                "-I.",
                "-IRegression",
            },
        });
    }

    // Install the library (contains all .o files)
    b.installArtifact(regressionCLib);

    const test_exe = b.addExecutable(.{
        .name = "test",
        .target = target,
        .optimize = optimize,
    });

    // Link against the previously built library and libm
    test_exe.linkLibrary(regressionCLib);
    test_exe.linkSystemLibrary("m");

    test_exe.addCSourceFile(.{
        .file = b.path("./Test/test.c"),
        .flags = &.{
            "-g",
            "-Wall",
            "-Wextra",
            "-Werror",
            "-I.",
            "-IRegression",
        },
    });

    // Make the test executable available
    b.installArtifact(test_exe);

    // Add a "test" step to run your test executable
    const test_step = b.step("test", "Run the tests");
    const run_test = b.addRunArtifact(test_exe);
    test_step.dependOn(&run_test.step);
}
