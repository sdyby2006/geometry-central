namespace geometrycentral {
namespace surface {

// Face areas
inline double EdgeLengthGeometry::faceArea(Face f) const {
  // WARNING: Logic duplicated between cached and immediate version
  Halfedge he = f.halfedge();
  double a = inputEdgeLengths[he.edge()];
  he = he.next();
  double b = inputEdgeLengths[he.edge()];
  he = he.next();
  double c = inputEdgeLengths[he.edge()];

  GC_SAFETY_ASSERT(he.next() == f.halfedge(), "faces mush be triangular");

  // Herons formula
  double s = (a + b + c) / 2.0;
  double arg = s * (s - a) * (s - b) * (s - c);
  arg = std::fmax(0., arg);
  double area = std::sqrt(arg);
  return area;
}

// Corner angles
inline double EdgeLengthGeometry::cornerAngle(Corner c) const {
  // WARNING: Logic duplicated between cached and immediate version
  Halfedge heA = c.halfedge();
  Halfedge heOpp = heA.next();
  Halfedge heB = heOpp.next();

  GC_SAFETY_ASSERT(heB.next() == heA, "faces mush be triangular");

  double lOpp = inputEdgeLengths[heOpp.edge()];
  double lA = inputEdgeLengths[heA.edge()];
  double lB = inputEdgeLengths[heB.edge()];

  double q = (lA * lA + lB * lB - lOpp * lOpp) / (2. * lA * lB);
  q = clamp(q, -1.0, 1.0);
  double angle = std::acos(q);

  return angle;
}


inline double EdgeLengthGeometry::halfedgeCotanWeight(Halfedge heI) const {
  // WARNING: Logic duplicated between cached and immediate version
  double cotSum = 0.;

  if (heI.isInterior()) {
    Halfedge he = heI;
    double l_ij = inputEdgeLengths[he.edge()];
    he = he.next();
    double l_jk = inputEdgeLengths[he.edge()];
    he = he.next();
    double l_ki = inputEdgeLengths[he.edge()];
    he = he.next();
    GC_SAFETY_ASSERT(he == heI, "faces mush be triangular");
    double area = faceArea(he.face());
    double cotValue = (-l_ij * l_ij + l_jk * l_jk + l_ki * l_ki) / (4. * area);
    return cotValue / 2;
  } else {
    return 0.;
  }
}

inline double EdgeLengthGeometry::edgeCotanWeight(Edge e) const {
  return halfedgeCotanWeight(e.halfedge()) + halfedgeCotanWeight(e.halfedge().twin());
}

} // namespace surface
} // namespace geometrycentral
