/*----------------------------------------------------------------------------
 *
 *   Copyright (C) 2016 - 2023 Antonio Augusto Alves Junior
 *
 *   This file is part of Hydra Data Analysis Framework.
 *
 *   Hydra is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Hydra is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Hydra.  If not, see <http://www.gnu.org/licenses/>.
 *
 *---------------------------------------------------------------------------*/

/*
 * Ipatia_plus_argus.inl
 *
 *  Created on: 26/07/2018
 *      Author: Antonio Augusto Alves Junior
 */

#ifndef IPATIA_PLUS_ARGUS_INL_
#define IPATIA_PLUS_ARGUS_INL_

#include <iostream>
#include <assert.h>
#include <time.h>
#include <chrono>
#include <random>
#include <algorithm>

//command line
#include <tclap/CmdLine.h>

//this lib
#include <hydra/device/System.h>
#include <hydra/Function.h>
#include <hydra/Lambda.h>
#include <hydra/FunctorArithmetic.h>
#include <hydra/Random.h>
#include <hydra/LogLikelihoodFCN.h>
#include <hydra/Parameter.h>
#include <hydra/UserParameters.h>
#include <hydra/Pdf.h>
#include <hydra/AddPdf.h>
#include <hydra/Filter.h>
#include <hydra/DenseHistogram.h>
#include <hydra/functions/Ipatia.h>
#include <hydra/functions/ArgusShape.h>
#include <hydra/Placeholders.h>


//Minuit2
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnUserParameterState.h"
#include "Minuit2/MnPrint.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnMinimize.h"

// * Include classes from ROOT to fill
#ifdef _ROOT_AVAILABLE_

#include <TROOT.h>
#include <TH1D.h>
#include <TApplication.h>
#include <TCanvas.h>

#endif //_ROOT_AVAILABLE_


using namespace ROOT::Minuit2;
using namespace hydra::placeholders;
using namespace hydra::arguments;

declarg( _X, double)

int main(int argv, char** argc)
{
	size_t nentries = 0;

	try {

		TCLAP::CmdLine cmd("Command line arguments for ", '=');

		TCLAP::ValueArg<size_t> EArg("n", "number-of-events","Number of events", true, 10e6, "size_t");
		cmd.add(EArg);

		// Parse the argv array.
		cmd.parse(argv, argc);

		// Get the value parsed by each arg.
		nentries = EArg.getValue();

	}
	catch (TCLAP::ArgException &e)  {
		std::cerr << "error: " << e.error() << " for arg " << e.argId()
														<< std::endl;
	}

	//-----------------
    // some definitions
    double min   =  5.20;
    double max   =  5.30;


	//===========================
    //fit model gaussian + argus

	//Ipatia
	//core
	auto mu    = hydra::Parameter::Create("mu").Value(5.28).Error(0.0001).Limits(5.27,5.29);
	auto sigma = hydra::Parameter::Create("sigma").Value(0.0026).Error(0.0001).Limits(0.0020,0.0028);
	//left tail
	auto L1    = hydra::Parameter::Create("L1").Value(0.5).Error(0.0001).Limits(0.1, 1.0); // decay speed
	auto N1    = hydra::Parameter::Create("N1").Value(2.0).Error(0.0001).Limits(4.5, 6.5); // tail deepness
	//right tail
	auto L2    = hydra::Parameter::Create("L2").Value(1.2).Error(0.0001).Limits(0.5, 1.5);// decay speed
	auto N2    = hydra::Parameter::Create("N2").Value(9.5).Error(0.0001).Limits(8.5, 10.5);// tail deepness
	//peakness
	auto alfa  = hydra::Parameter::Create("alfa").Value(-6.5).Error(0.0001).Limits(-7.0, -6.0);
	auto beta  = hydra::Parameter::Create("beta").Value(1.0).Error(0.0001).Limits(0.5, 1.5);


	//ipatia function evaluating on the first argument
	auto Signal_PDF = hydra::make_pdf(hydra::Ipatia<_X>(mu, sigma,L1,N1,L2,N2,alfa,beta),
			hydra::AnalyticalIntegral<hydra::Ipatia<_X>>(min,  max));

    //-------------------------------------------
	//Argus
    //parameters
    auto  m0     = hydra::Parameter::Create("M0").Value(5.291).Error(0.0001).Limits(5.28, 5.3);
    auto  slope  = hydra::Parameter::Create("Slope").Value(-20.0).Error(0.0001).Limits(-30.0, -10.0);
    auto  power  = hydra::Parameter::Create("Power").Value(0.5).Fixed();

    //argus function evaluating on the first argument
    auto Background_PDF = hydra::make_pdf( hydra::ArgusShape<_X>(m0, slope, power),
    		hydra::AnalyticalIntegral<hydra::ArgusShape<_X>>(min, max));

    //------------------
    //yields
	hydra::Parameter N_Signal("N_Signal"        ,500, 100, 100 , nentries) ;
	hydra::Parameter N_Background("N_Background",2000, 100, 100 , nentries) ;

	//make model
	auto model = hydra::add_pdfs( {N_Signal, N_Background}, Signal_PDF, Background_PDF);
	model.SetExtended(1);

#ifdef _ROOT_AVAILABLE_

	TH1D 	hist_data("data"	, "Gaussian + ARGUS", 100, min, max);
	TH1D 	hist_fit("fit"  	, "Gaussian + ARGUS", 100, min, max);
	TH1D 	hist_signal("signal", "Gaussian + ARGUS", 100, min, max);
	TH1D 	hist_background("background"  , "Gaussian + ARGUS", 100, min, max);

#endif //_ROOT_AVAILABLE_

	//scope begin
	{

		//1D device buffer
		hydra::device::vector<double>  data(nentries);

		//-------------------------------------------------------
		// Generate data
		auto range = hydra::sample(data, min, max, model.GetFunctor());

		std::cout<< std::endl<< "Generated data (10 first elements):"<< std::endl;
		for(size_t i=0; i< 10; i++)
			std::cout << "[" << i << "] :" << range[i] << std::endl;

		std::cout<< std::endl<< "Total dataset size :"<< range.size() << std::endl;

		//make model and fcn
		auto fcn = hydra::make_loglikehood_fcn( model, range );

		//-------------------------------------------------------
		//fit
		ROOT::Minuit2::MnPrint::SetLevel(3);

		MnStrategy strategy(2);

		// create Migrad minimizer
		MnMigrad migrad_d(fcn, fcn.GetParameters().GetMnState() ,  strategy);

		//print parameters prior the fit
		std::cout<<fcn.GetParameters().GetMnState()<<std::endl;

		// ... Minimize and profile the time

		auto start_d = std::chrono::high_resolution_clock::now();

		FunctionMinimum minimum_d =  FunctionMinimum(migrad_d(500000, 50));

		auto end_d = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double, std::milli> elapsed_d = end_d - start_d;

		// output
		std::cout<< "Minimum: " << minimum_d << std::endl;

		//time
		std::cout << "-----------------------------------------"<<std::endl;
		std::cout << "| [Fit] GPU Time (ms) ="<< elapsed_d.count() <<std::endl;
		std::cout << "-----------------------------------------"<<std::endl;

		//--------------------------------------------
		hydra::DenseHistogram<double, 1, hydra::device::sys_t> Hist_Data(100, min, max);
		Hist_Data.Fill( range );




#ifdef _ROOT_AVAILABLE_


		//data
		for(size_t i=0;  i<100; i++)
			hist_data.SetBinContent(i+1, Hist_Data.GetBinContent(i));

		//fit
		for (size_t i=0 ; i<=100 ; i++) {
			double x = hist_fit.GetBinCenter(i);
	        hist_fit.SetBinContent(i, fcn.GetPDF()(x) );
		}
		hist_fit.Scale(hist_data.Integral()/hist_fit.Integral() );

		//signal component
		auto   signal          = fcn.GetPDF().PDF(_0);
		double signal_fraction = fcn.GetPDF().Coefficient(0)/fcn.GetPDF().GetCoefSum();
		for (size_t i=0 ; i<=100 ; i++) {
			double x = hist_signal.GetBinCenter(i);
			hist_signal.SetBinContent(i, signal(x) );
		}
		hist_signal.Scale(hist_data.Integral()*signal_fraction/hist_signal.Integral());

		//signal component
		auto   background          = fcn.GetPDF().PDF(_1);
		double background_fraction = fcn.GetPDF().Coefficient(1)/fcn.GetPDF().GetCoefSum();
		for (size_t i=0 ; i<=100 ; i++) {
			double x = hist_background.GetBinCenter(i);
			hist_background.SetBinContent(i, background(x) );
		}
		hist_background.Scale(hist_data.Integral()*background_fraction/hist_background.Integral());




#endif //_ROOT_AVAILABLE_

	}//scope end


#ifdef _ROOT_AVAILABLE_

	TApplication *myapp=new TApplication("myapp",0,0);

	//draw histograms
	TCanvas canvas_d("canvas_d" ,"Distributions - Device", 500, 500);

	hist_data.Draw("e0");
	hist_data.SetStats(0);
	hist_data.SetLineColor(1);
	hist_data.SetLineWidth(2);

	hist_fit.Draw("histsameC");
	hist_fit.SetStats(0);
	hist_fit.SetLineColor(4);

	hist_signal.Draw("histsameC");
	hist_signal.SetStats(0);
	hist_signal.SetLineColor(3);

	hist_background.Draw("histsameC");
	hist_background.SetStats(0);
	hist_background.SetLineColor(2);
	hist_background.SetLineStyle(2);

	hist_fit.Draw("histsameC");
	hist_data.Draw("e0same");


	myapp->Run();

#endif //_ROOT_AVAILABLE_

	return 0;

}




#endif /* IPATIA_PLUS_ARGUS_INL_ */
