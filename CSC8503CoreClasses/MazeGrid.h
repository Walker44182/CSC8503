#pragma once
#include "NavigationMap.h"
#include "NavigationGrid.h"
#include <string>

namespace NCL {
	namespace CSC8503 {
		class MazeGrid1 : public NavigationGrid {
		public:
			MazeGrid1(const std::string& filename);
			~MazeGrid1();

			bool FindPath(const Vector3& from, const Vector3& to, NavigationPath& outPath) override;
		};

		class MazeGrid2 : public NavigationGrid {
		public:
			MazeGrid2(const std::string& filename);
			~MazeGrid2();

			bool FindPath(const Vector3& from, const Vector3& to, NavigationPath& outPath) override;
		};
	}
}