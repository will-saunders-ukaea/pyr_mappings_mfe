#include <LibUtilities/BasicUtils/SessionReader.h>
#include <SolverUtils/Driver.h>
#include <memory>
#include <iostream>
#include <cmath>

// Helper functions
template <typename U> inline void nprint_recurse(int flag, U next) {
  if (flag) {
    std::cout << " ";
  }
  std::cout << next << std::endl;
}

template <typename U, typename... T>
inline void nprint_recurse(int flag, U next, T... args) {
  if (flag) {
    std::cout << " ";
  }
  std::cout << next;
  nprint_recurse(++flag, args...);
}

template <typename... T> inline void nprint(T... args) {
  nprint_recurse(0, args...);
}

template <typename T, typename U>
inline double l2_norm(T &t, U &u){
  const double d0 = t[0] - u[0];
  const double d1 = t[1] - u[1];
  const double d2 = t[2] - u[2];
  return std::sqrt(d0 * d0 + d1 * d1 + d2 * d2);
}

using namespace std;
using namespace Nektar;
using namespace Nektar::SolverUtils;
using namespace Nektar::SpatialDomains;

auto make_problem_pyramid(){

  std::map<int, std::shared_ptr<PointGeom>> vertices;
  vertices[8] = std::make_shared<PointGeom>(3, 8, -1.0, -1.0, -0.6);
  vertices[9] = std::make_shared<PointGeom>(3, 9, -0.5, -1.0, -0.6);
  vertices[10] = std::make_shared<PointGeom>(
    3, 10, -0.4794834360000000, -0.4749725820000000, -0.6614790460000000);
  vertices[11] = std::make_shared<PointGeom>(3, 11, -1.0, -0.5, -0.6);
  vertices[107] = std::make_shared<PointGeom>(
    3, 107, -0.8043345620000000, -0.7478333320000000, -0.4296764360000000);
  
  std::map<int, std::shared_ptr<SegGeom>> edges;
  std::shared_ptr<PointGeom> ends[2];
  
  ends[0] = vertices.at(8);
  ends[1] = vertices.at(9);
  edges[16] = std::make_shared<SegGeom>(16, 3, ends, nullptr);
  ends[0] = vertices.at(9);
  ends[1] = vertices.at(10);
  edges[17] = std::make_shared<SegGeom>(17, 3, ends, nullptr);
  ends[0] = vertices.at(10);
  ends[1] = vertices.at(11);
  edges[18] = std::make_shared<SegGeom>(18, 3, ends, nullptr);
  ends[0] = vertices.at(11);
  ends[1] = vertices.at(8);
  edges[19] = std::make_shared<SegGeom>(19, 3, ends, nullptr);
  ends[0] = vertices.at(107);
  ends[1] = vertices.at(8);
  edges[402] = std::make_shared<SegGeom>(402, 3, ends, nullptr);
  ends[0] = vertices.at(9);
  ends[1] = vertices.at(107);
  edges[262] = std::make_shared<SegGeom>(262, 3, ends, nullptr);
  ends[0] = vertices.at(10);
  ends[1] = vertices.at(107);
  edges[264] = std::make_shared<SegGeom>(264, 3, ends, nullptr);
  ends[0] = vertices.at(11);
  ends[1] = vertices.at(107);
  edges[317] = std::make_shared<SegGeom>(317, 3, ends, nullptr);
  
  std::map<int, std::shared_ptr<Geometry2D>> faces;
  std::shared_ptr<SegGeom> ee[4];

  ee[0] = edges.at(16);
  ee[1] = edges.at(17);
  ee[2] = edges.at(18);
  ee[3] = edges.at(19);
  faces[10] = std::make_shared<QuadGeom>(10, ee, nullptr);
  ee[0] = edges.at(16);
  ee[1] = edges.at(402);
  ee[2] = edges.at(262);
  faces[410] = std::make_shared<TriGeom>(410, ee, nullptr);
  ee[0] = edges.at(17);
  ee[1] = edges.at(264);
  ee[2] = edges.at(262);
  faces[223] = std::make_shared<TriGeom>(223, ee, nullptr);
  ee[0] = edges.at(18);
  ee[1] = edges.at(264);
  ee[2] = edges.at(317);
  faces[287] = std::make_shared<TriGeom>(287, ee, nullptr);
  ee[0] = edges.at(19);
  ee[1] = edges.at(317);
  ee[2] = edges.at(402);
  faces[382] = std::make_shared<TriGeom>(382, ee, nullptr);
  
  std::shared_ptr<Geometry2D> ff[5];
  ff[0] = faces.at(10);
  ff[1] = faces.at(410);
  ff[2] = faces.at(223);
  ff[3] = faces.at(287);
  ff[4] = faces.at(382);
  auto pyr = std::make_shared<PyrGeom>(0, ff);
 
  pyr->GetGeomFactors();
  pyr->Setup();

  return pyr;
}


auto make_linear_problem_pyramid(){

  int vertex_ids[5] = { 38, 53, 56, 41, 101 };
  NekDouble coords[15] = {
    0,   0,  -0.6, 
    0.5, 0.0, -0.6, 
    0.55, 0.5425, -0.6, 
    0.0, 0.5, -0.6, 
    0.233551, 0.20215, -0.372506
  };
  int edge_ids[8] = {116, 123, 124, 86,  239, 231, 230, 249};
  int edges[16] = {53, 38, 53, 56, 56, 41, 38, 41, 101, 38,53, 101, 56, 101, 41, 101};

  int face_edge_counts[5] = {4,3,3,3,3};
  int face_ids[5] = {90, 185, 177, 213, 198};
  int faces[] = {116, 123, 124, 86, 116, 239, 231, 123, 231, 230, 124, 230, 249, 86, 249, 239};

  int pyr_faces[] = {90, 185, 177, 213, 198};


  std::map<int, std::shared_ptr<PointGeom>> m_vertices;
  std::map<int, std::shared_ptr<SegGeom>> m_edges;
  std::map<int, std::shared_ptr<Geometry2D>> m_faces;

  std::shared_ptr<PointGeom> ends[2];
  std::shared_ptr<SegGeom> ee[4];
  std::shared_ptr<Geometry2D> ff[5];

  for(int vx=0 ; vx<5 ; vx++){
    const int id = vertex_ids[vx];
    NekDouble x = coords[vx * 3 + 0];
    NekDouble y = coords[vx * 3 + 1];
    NekDouble z = coords[vx * 3 + 2];
    m_vertices[id] = std::make_shared<PointGeom>(3, id, x,y,z);
  }

  for(int vx=0 ; vx<8 ; vx++){
    const int id = edge_ids[vx];
    ends[0] = m_vertices.at(edges[2 * vx + 0]);
    ends[1] = m_vertices.at(edges[2 * vx + 1]);
    m_edges[id] = std::make_shared<SegGeom>(id, 3, ends, nullptr);
  }

  int index = 0;
  for(int vx=0 ; vx<5 ; vx++){
    const int id = face_ids[vx];
    const int count = face_edge_counts[vx];
    for(int ix=0 ; ix<count ; ix++){
      ee[ix] = m_edges.at(faces[index++]);
    }
    if (count == 3){
      m_faces[id] = std::make_shared<TriGeom>(id, ee, nullptr);
    } else if (count ==4){
      m_faces[id] = std::make_shared<QuadGeom>(id, ee, nullptr);
    } else {
      nprint("ERROR BAD EDGE COUNT");
    }
    ff[vx] = m_faces.at(id);
  }

  auto pyr = std::make_shared<PyrGeom>(1, ff);
  pyr->GetGeomFactors();
  pyr->Setup();

  return pyr;
}


int main(int argc, char ** argv){
  auto pyr_non_linear = make_problem_pyramid();
  auto pyr_linear = make_linear_problem_pyramid();
  
  std::vector<std::shared_ptr<PyrGeom>> geoms = {pyr_non_linear, pyr_linear};
  
  for (auto pyr : geoms){

    Array<OneD, NekDouble> xi_print(3);
    xi_print[0] = -0.125;
    xi_print[1] = -0.125;
    xi_print[2] =  0.125;

    // Check that X and X^-1 are consistent
    nprint("\nConsistency check, mapping point from physical to local and back to physical:");
    // Create a global (physical space) coordinate
    Array<OneD, NekDouble> global_coord(3);
    for (int dx = 0; dx < 3; dx++) {
      global_coord[dx] = pyr->GetCoord(dx, xi_print);
    }

    // map back to reference space
    Array<OneD, NekDouble> xi(3);
    NekDouble dist;
    auto is_contained = pyr->ContainsPoint(global_coord, xi, 1.0e-12, dist);
    nprint("is_contained:", (int) is_contained);

    // map the local coord back to global space
    Array<OneD, NekDouble> global_coord_check(3);
    for (int dx = 0; dx < 3; dx++) {
      global_coord_check[dx] = pyr->GetCoord(dx, xi);
    }

    // print both global coordinates
    nprint("0:", global_coord[0], global_coord[1], global_coord[2]);
    nprint("1:", global_coord_check[0], global_coord_check[1], global_coord_check[2]);
    nprint("error:", l2_norm(global_coord_check, global_coord));

  }

  return 0;
}


