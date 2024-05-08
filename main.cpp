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

class Pyr : public SpatialDomains::PyrGeom {
public:
  template <typename ...T>
  Pyr(
    T... t
  ):
    PyrGeom(t...)
  {}

  inline double print_coord_diff(Array<OneD, NekDouble> &Lcoords){
    Array<OneD, NekDouble> c_old(3);
    for (int dx = 0; dx < 3; dx++) {
      c_old[dx] = GetCoord(dx, Lcoords);
    }
    nprint("Lcoord:        ", Lcoords[0], Lcoords[1], Lcoords[2]);
    nprint("OLD   physical:", c_old[0], c_old[1], c_old[2]);
    
    /**
     * This is taken from the straight edge newton iteration code for X^-1 that
     * appeared around 5.4.0.
     */
    Array<OneD, NekDouble> var(8, 1.);
    NekDouble xmap, ymap, zmap;
    auto lambda = [&](auto xi){
      var[1] = xi[0];
      var[2] = xi[1];
      var[3] = xi[2];
      var[4] = xi[0] * xi[1];
      var[5] = xi[1] * xi[2];
      var[6] = xi[0] * xi[2];
      var[7] = var[4] * xi[2];
      // calculate the global point corresponding to xi
      xmap =
          Vmath::Dot(m_isoParameter[0].size(), var, 1, m_isoParameter[0], 1);
      ymap =
          Vmath::Dot(m_isoParameter[0].size(), var, 1, m_isoParameter[1], 1);
      zmap =
          Vmath::Dot(m_isoParameter[0].size(), var, 1, m_isoParameter[2], 1);
    };
    
    lambda(Lcoords);
    nprint("NEW   physical:", xmap, ymap, zmap);

    double n[3] = {xmap, ymap, zmap};
    return l2_norm(n, c_old);
  }

};


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
  auto pyr = std::make_shared<Pyr>(0, ff);
 
  pyr->GetGeomFactors();
  pyr->Setup();

  return pyr;
}


int main(int argc, char ** argv){
  auto pyr = make_problem_pyramid();
  
  // Check the maps from reference space to physical space are consistent.
  nprint("Vertices match:");
  Array<OneD, NekDouble> xi_print(3);
  xi_print[0] = -1.0;
  xi_print[1] = -1.0;
  xi_print[2] = -1.0;
  pyr->print_coord_diff(xi_print);
  xi_print[0] =  1.0;
  xi_print[1] = -1.0;
  xi_print[2] = -1.0;
  pyr->print_coord_diff(xi_print);
  xi_print[0] =  1.0;
  xi_print[1] =  1.0;
  xi_print[2] = -1.0;
  pyr->print_coord_diff(xi_print);
  xi_print[0] = -1.0;
  xi_print[1] =  1.0;
  xi_print[2] = -1.0;
  pyr->print_coord_diff(xi_print);
  xi_print[0] = -1.0;
  xi_print[1] = -1.0;
  xi_print[2] =  1.0;
  pyr->print_coord_diff(xi_print);

  nprint("\nInterior differs:");
  xi_print[0] =  0.0;
  xi_print[1] =  0.0;
  xi_print[2] =  0.0;
  pyr->print_coord_diff(xi_print);
  xi_print[0] = -0.2;
  xi_print[1] = -0.2;
  xi_print[2] = -0.2;
  pyr->print_coord_diff(xi_print);
  xi_print[0] = -0.125;
  xi_print[1] = -0.125;
  xi_print[2] =  0.125;
  pyr->print_coord_diff(xi_print);

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
  
  return 0;
}


