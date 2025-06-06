//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// This example is provided by the Geant4-DNA collaboration
// chem6 example is derived from chem4 and chem5 examples
//
// Any report or published results obtained using the Geant4-DNA software
// shall cite the following Geant4-DNA collaboration publication:
// J. Appl. Phys. 125 (2019) 104301
// Med. Phys. 45 (2018) e722-e739
// J. Comput. Phys. 274 (2014) 841-882
// Med. Phys. 37 (2010) 4692-4708
// Int. J. Model. Simul. Sci. Comput. 1 (2010) 157-178
// The Geant4-DNA web site is available at http://geant4-dna.org
//
// Authors: W. G. Shin and S. Incerti (CENBG, France)
//
// $Id$
//
/// \file ScoreSpecies.hh
/// \brief Definition of the ScoreSpecies class

#ifndef CHEM6_ScoreSpecies_h
#define CHEM6_ScoreSpecies_h 1

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"
#include <set>
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UImessenger.hh"

class G4VAnalysisManager;
class G4MolecularConfiguration;

/** \file ScoreSpecies.hh*/

// Description:
//   This is a primitive scorer class for scoring the radiolitic species
// produced after irradiation in a water volume
//
// Created: 2015-10-27  by M. Karamitros,
// modified: 2016-03-16 by P. Piersimoni

class ScoreSpecies : public G4VPrimitiveScorer,
                     public G4UImessenger
{
public:
  ScoreSpecies(G4String name, G4int depth=0);

  virtual ~ScoreSpecies();
  
  /** Add a time at which the number of species should be recorded.
      Default times are set up to 1 microsecond.*/
  inline void AddTimeToRecord(double time)
  {
    fTimeToRecord.insert(time);
  }
  
  /**  Remove all times to record, must be reset by user.*/
  inline void ClearTimeToRecord()
  {
    fTimeToRecord.clear();
  }

  /** Get number of recorded events*/
  inline int GetNumberOfRecordedEvents() const
  {
    return fNEvent;
  }

  /** Write results to whatever chosen file format*/
  void WriteWithAnalysisManager(G4VAnalysisManager*);
  
  struct SpeciesInfo
  {
    SpeciesInfo()
    {
      fNumber = 0;
      fG = 0.;
      fG2 = 0.;
      fLET = 0.;
    }
    SpeciesInfo(const SpeciesInfo& right) // Species A(B);
    {
      fNumber = right.fNumber;
      fG = right.fG;
      fG2 = right.fG2;
      fLET = right.fLET;
    }
    SpeciesInfo& operator=(const SpeciesInfo& right) // A = B
    {
      if(&right == this) return *this;
      fNumber = right.fNumber;
      fG = right.fG;
      fG2 = right.fG2;
      fLET = right.fLET;
      return *this;
    }
    int fNumber;
    double fG;
    double fG2;
    double fLET;
  };
  void DumpResultsToCSV(const std::string& filename);

public:
  static void SetCurrentLET(double let) { fCurrentLET = let; }
  static double GetCurrentLET() { return fCurrentLET; }

private:
  static double fCurrentLET;
  
  
private:
  typedef const G4MolecularConfiguration Species;
  typedef std::map<Species*, SpeciesInfo>  InnerSpeciesMap;
  typedef std::map<double, InnerSpeciesMap> SpeciesMap;
  SpeciesMap fSpeciesInfoPerTime;

  std::set<G4double> fTimeToRecord;
  
  int fNEvent; // number of processed events
  double fEdep; // total energy deposition
  G4String fOutputType; // output type 

protected:
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

public:
  virtual void Initialize(G4HCofThisEvent*);
  virtual void EndOfEvent(G4HCofThisEvent*);
  virtual void DrawAll();
  virtual void PrintAll();
  /** Method used in multithreading mode in order to merge 
      the results*/
  virtual void AbsorbResultsFromWorkerScorer(G4VPrimitiveScorer* );
  virtual void OutputAndClear();
  virtual void SetNewValue(G4UIcommand*, G4String);
  
  SpeciesMap GetSpeciesInfo() {return fSpeciesInfoPerTime;}

private:
  G4int fHCID;
  G4THitsMap<G4double>* fEvtMap;

  G4int fRunID;
  G4UIdirectory* fSpeciesdir;
  G4UIcmdWithAnInteger* fTimeBincmd;
  G4UIcmdWithADoubleAndUnit* fAddTimeToRecordcmd;
};
#endif
