#include <cstdlib>
#include <map>

#include "utility.hpp"

using namespace llvm;

static void ProcessModule(Module &M) {
  for (Function &F : M.functions()) {
    std::map<uint32_t, size_t> instruction_calls;
    for (BasicBlock &BB : F) {
      for (Instruction &I : BB) {
        instruction_calls[I.getOpcode()]++;
      }
    }

    outs() << "Function " << F.getName() << "\n";
    for (const auto &[key, value] : instruction_calls) {
      outs() << Instruction::getOpcodeName(key) << " " << value << "\n";
    }
  }
}

int main(int argc, char **argv) {
  // Parse arguments
  if (argc != 2) {
    printf("Usage: %s in.bc\n", std::filesystem::path(argv[0]).filename().c_str());
    return EXIT_FAILURE;
  }
  auto inFile = argv[1];

  // Load module
  LLVMContext C;
  auto M = LoadModule(C, inFile);

  // Process module
  try {
    ProcessModule(*M);
  } catch (const std::exception &x) {
    outs() << x.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
