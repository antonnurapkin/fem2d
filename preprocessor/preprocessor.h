#include <fstream>
#include <string>
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
    std::vector<IElement*> elements;
	std::vector<Node> nodes;
	std::vector<Material> materials;
	std::vector<Force> forces;
	std::vector<Support> supports;

	std::string path_to_input_file;
public:
    Preprocessor(std::string path_to_input_file);
    ~Preprocessor();
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