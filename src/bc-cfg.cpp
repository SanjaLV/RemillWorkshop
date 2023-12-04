#include <cstdlib>
#include <map>

#include "utility.hpp"
#include "llvm/IR/CFG.h"

using namespace llvm;

namespace Sanja::Graph {
struct Edge {
  std::string from, to;

  template <typename T>
  Edge (T a, T b) : from(a), to(b) {}
};

typedef std::vector<Edge> Edges;
} // namespace Sanja::Graph

static void ProcessModule(Module &M) {
  for (Function &F : M.functions()) {

    Sanja::Graph::Edges edges;

    for (BasicBlock &BB : F) {
      for (BasicBlock *Pred : predecessors(&BB)) {
        edges.emplace_back(Pred->getName(), BB.getName());
      }
    }

    outs() << "Function " << F.getName() << "\n";
    for (const auto &[a, b] : edges) {
      outs() << a << " -> " << b << "\n";
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
