import FWCore.ParameterSet.Config as cms

from Geometry.TrackerGeometryBuilder.StackedTrackerGeometry_cfi import *

from SimTracker.TrackTriggerAssociation.TTClusterAssociation_cfi import *
from SimTracker.TrackTriggerAssociation.TTStubAssociation_cfi import *
from SimTracker.TrackTriggerAssociation.TTTrackAssociation_cfi import *

TrackTriggerAssociatorClustersStubs = cms.Sequence(TTClusterAssociatorFromPixelDigis*TTStubAssociatorFromPixelDigis)
TrackTriggerAssociatorTracks = cms.Sequence(TTTrackAssociatorFromPixelDigis)
TrackTriggerAssociatorComplete = cms.Sequence(TTClusterAssociatorFromPixelDigis*TTStubAssociatorFromPixelDigis*TTTrackAssociatorFromPixelDigis)

