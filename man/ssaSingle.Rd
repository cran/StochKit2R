% Generated by roxygen2 (4.1.0): do not edit by hand
% Please edit documentation in R/ssaSingle.R
\name{ssaSingle}
\alias{ssaSingle}
\title{Gillespie Stochastic Simulation Algorithm single trajectory}
\usage{
ssaSingle(modelFile, outputFile, startTime, endTime, seed = NULL)
}
\arguments{
\item{modelFile}{Character string with path to StochKit2 .xml model file}

\item{outputFile}{Character string with path to output file.}

\item{startTime}{Simulation start time}

\item{endTime}{Simulation end time}

\item{seed}{Seed the random number generator. By default the seed is determined by the R random number generator, so the seed can also be set by calling \code{set.seed} in R immediately before calling \code{ssaSingle}}
}
\description{
\code{ssaSingle} Run a single SSA trajectory and write output
data to \code{outputFile}. Stores a row of data for every reaction event.
}
\examples{
\dontrun{
#example using included dimer_decay.xml file
#output written to file single_output.txt (created in current working directory)
ssaSingle(system.file("dimer_decay.xml",package="StochKit2R"),
         "single_output.txt",0,10)
}
}

