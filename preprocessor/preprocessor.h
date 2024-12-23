#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <optional>
#include "elem_service/IElement.h"
#include "node/Node.h"
#include "material/Material.h"
#include "boundaries/Force.h"
#include "boundaries/Support.h"
#include "elem_service/ElemParams.h"
#include "elem_service/ElemCreator.h"
#include "utils/tools.h"
#include "utils/Error.h"

class Preprocessor{
private:
	std::string path_to_input_file;

    std::vector<IElement*> elements;
	std::vector<Node> nodes;
	std::vector<Material> materials;
	std::vector<Force> forces;
	std::vector<Support> supports;
public:
    Preprocessor(int number_params, char** params);
    ~Preprocessor();

	std::string getPathToConfig(int number_params, char** params);
    void readConfig();

	ElemParams createElemParams(std::map<std::string, std::optional<double>> elem_data, double geometry);

	std::vector<IElement*> getElements();
	std::vector<Node> getNodes();
	std::vector<Material> getMaterials();
	std::vector<Force> getForces();
	std::vector<Support> getSupports();

	std::vector<int> getDofIndexes(Support support);

	Node getNodeByIndex(int index);
	Material getMaterialByIndex(int index);
	
};