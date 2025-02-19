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
 * multidimensional_fit.inl
 *
 *  Created on: 01/09/2017
 *      Author: Antonio Augusto Alves Junior
 */

#ifndef MULTIDIMENSIONAL_FIT_INL_
#define MULTIDIMENSIONAL_FIT_INL_

/**
 * \example multidimensional_fit.inl
 *
 * This example implements a 3D dimensional Gaussian fit.
 */


#include <iostream>
#include <assert.h>
#include <time.h>
#include <chrono>
#include <random>
#include <algorithm>
#include <future>
//command line
#include <tclap/CmdLine.h>

//this lib
#include <hydra/device/System.h>
#include <hydra/host/System.h>
#include <hydra/Function.h>
#include <hydra/Lambda.h>
#include <hydra/Random.h>
#include <hydra/Algorithm.h>
#include <hydra/Tuple.h>
#include <hydra/Distance.h>
#include <hydra/multiarray.h>
#include <hydra/multivector.h>
#include <hydra/LogLikelihoodFCN.h>
#include <hydra/Parameter.h>
#include <hydra/UserParameters.h>
#include <hydra/Pdf.h>
#include <hydra/AddPdf.h>
#include <hydra/Filter.h>
#include <hydra/Plain.h>
#include <hydra/DenseHistogram.h>
#include <hydra/functions/Gaussian.h>
#include <hydra/RandomFill.h>

//Minuit2
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnUserParameterState.h"
#include "Minuit2/MnPrint.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnMinimize.h"
#include "Minuit2/MnMinos.h"
#include "Minuit2/MnContours.h"
#include "Minuit2/CombinedMinimizer.h"
#include "Minuit2/MnPlot.h"
#include "Minuit2/MinosError.h"
#include "Minuit2/ContoursError.h"
#include "Minuit2/VariableMetricMinimizer.h"

/*-------------------------------------
 * Include classes from ROOT to fill
 * and draw histograms and plots.
 *-------------------------------------
 */
#ifdef _ROOT_AVAILABLE_

#include <TROOT.h>
#include <TH1D.h>
#include <TH3D.h>
#include <TApplication.h>
#include <TCanvas.h>

#endif //_ROOT_AVAILABLE_

using namespace ROOT::Minuit2;
using namespace hydra::arguments;


declarg(_X, double)
declarg(_Y, double)
declarg(_Z, double)


int main(int argv, char** argc) {
	size_t nentries = 0;

	try {

		TCLAP::CmdLine cmd("Command line arguments for ", '=');

		TCLAP::ValueArg < size_t
				> EArg("n", "number-of-events", "Number of events", true, 10e6,
						"size_t");
		cmd.add(EArg);

		// Parse the argv array.
		cmd.parse(argv, argc);

		// Get the value parsed by each arg.
		nentries = EArg.getValue();

	} catch (TCLAP::ArgException &e) {
		std::cerr << "error: " << e.error() << " for arg " << e.argId()
				<< std::endl;
	}
//-----------------
	// some definitions
	double min = -6.0;
	double max =  6.0;

	double meanx = 0.0;
	double sigmax = 0.5;

	double meany = 0.0;
	double sigmay = 1.5;

	double meanz = 0.0;
	double sigmaz = 1.0;

	//______________________________________________________________
    // mean of gaussian in x-direction
	hydra::Parameter meanx_p =
			hydra::Parameter::Create().Name("Mean_X").Value(0.0).Error(0.0001).Limits(
					-1.0, 1.0);

	// sigma of gaussian in x-direction
	hydra::Parameter sigmax_p =
			hydra::Parameter::Create().Name("Sigma_X").Value(2.0).Error(0.0001).Limits(0.1, 3.0);
	//______________________________________________________________
	// mean of gaussian in y-direction
	hydra::Parameter meany_p =
			hydra::Parameter::Create().Name("Mean_Y").Value(0.0).Error(0.0001).Limits(-1.0, 1.0);

	// sigma of gaussian in y-direction
	hydra::Parameter sigmay_p =
			hydra::Parameter::Create().Name("Sigma_Y").Value(2.0).Error(0.01).Limits(0.1, 3.0);

	//______________________________________________________________
	// mean of gaussian in z-direction
	hydra::Parameter meanz_p =
			hydra::Parameter::Create().Name("Mean_Z").Value(0.0).Error(0.0001).Limits(-1.0, 1.0);

	// sigma of gaussian in z-direction
	hydra::Parameter sigmaz_p =
			hydra::Parameter::Create().Name("Sigma_Z").Value(2.0).Error(0.0001).Limits(0.1, 3.0);

	//______________________________________________________________
	//fit function

	auto gaussian = hydra::wrap_lambda(
		[=] __hydra_dual__ 	(unsigned int npar, const hydra::Parameter* params, _X x, _Y y, _Z z ){

		double g = 1.0;

		double mean[3]  = {params[0].GetValue(), params[2].GetValue(), params[4].GetValue()};
		double sigma[3] = {params[1].GetValue(), params[3].GetValue(), params[5].GetValue()};
        double X[3]     = {x, y, z};

		for(size_t i=0; i<3; i++) {

			double m2 = (X[i] - mean[i] )*(X[i] - mean[i] );
			double s2 = sigma[i]*sigma[i];

			g *= exp(-m2/(2.0 * s2 ))/( sqrt(2.0*s2*PI));
		}

		return g;

	}, meanx_p, sigmax_p, meany_p, sigmay_p, meanz_p, sigmaz_p);

	hydra::Plain<3, hydra::device::sys_t> Integrator({min, min, min},{ max, max, max }, 50000);
	//make model and fcn
	auto model = hydra::make_pdf(gaussian, Integrator);


#ifdef _ROOT_AVAILABLE_

	TH3D hist_data_d("hist_data_d", "3D Gaussian - Data - Device",
			100, min, max,
			100, min, max,
			100, min, max );

	TH3D hist_mc_d("hist_mc_d", "3D Gaussian - Fit - Device",
			100, min, max,
			100, min, max,
			100, min, max );

#endif //_ROOT_AVAILABLE_

	//device
	//------------------------
	{

		//3D device/host buffer
		hydra::multivector< hydra::tuple<_X,_Y,_Z>,  hydra::device::sys_t> data_d(nentries);

		//-------------------------------------------------------
		// gaussian X
		auto GaussianX_Handler = std::async(std::launch::async,
				[&data_d, meanx, sigmax]( ) {

			hydra::fill_random(data_d.begin<_X>(), data_d.end<_X>(), hydra::Gaussian<_X>(meanx, sigmax), 159 );
		});

		//-------------------------------------------------------
		// gaussian X
		auto GaussianY_Handler = std::async(std::launch::async,
				[&data_d, meany, sigmay]( ) {

			hydra::fill_random(data_d.begin<_Y>(), data_d.end<_Y>(), hydra::Gaussian<_Y>(meany, sigmay), 753 );
		});

		//-------------------------------------------------------
		// gaussian X
		auto GaussianZ_Handler = std::async(std::launch::async,
				[&data_d, meanz, sigmaz]( ) {

			hydra::fill_random(data_d.begin<_Z>(), data_d.end<_Z>(), hydra::Gaussian<_Z>(meanz, sigmaz), 789 );
		});

		//-------------------------------------------------------
		//wait the threads to finish their tasks
		GaussianX_Handler.wait();
		GaussianY_Handler.wait();
		GaussianZ_Handler.wait();

		//-------------------------------------------------------

		std::cout<< std::endl<< "Generated data:"<< std::endl;
		for (size_t i = 0; i < 10; i++)
			std::cout << "[" << i << "] :" << data_d[i]
					<< std::endl;
		//filtering
		auto filter = hydra::wrap_lambda(
			[=] __hydra_dual__ ( _X x, _Y y, _Z z ){

			bool decision =
					((x > min) && (x < max ))&&
					((y > min) && (y < max ))&&
					((z > min) && (z < max ));

			return decision;

		});

		auto range  = hydra::filter(data_d, filter);

		std::cout << std::endl<< "Filtered data:" << std::endl;
		for (size_t i = 0; i < 10; i++)
			std::cout << "[" << i << "] :"
			          << range.begin()[i]
					  << std::endl;


		auto fcn = hydra::make_loglikehood_fcn(	model, range);

		fcn.GetPDF().PrintRegisteredParameters();

		//-------------------------------------------------------
		//fit
		ROOT::Minuit2::MnPrint::SetLevel(3);
		hydra::Print::SetLevel(hydra::WARNING);
		//minimization strategy
		MnStrategy strategy(2);

		// create Migrad minimizer
		MnMigrad migrad_d(fcn, fcn.GetParameters().GetMnState(), strategy);

		std::cout << fcn.GetParameters().GetMnState() << std::endl;

		// ... Minimize and profile the time

		auto start_d = std::chrono::high_resolution_clock::now();

		FunctionMinimum minimum_d = FunctionMinimum(migrad_d(5000, 5));

		auto end_d = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double, std::milli> elapsed_d = end_d - start_d;

		// output
		std::cout << "minimum: " << minimum_d << std::endl;

		//time
		std::cout << "-----------------------------------------" << std::endl;
		std::cout << "| GPU Time (ms) =" << elapsed_d.count()    << std::endl;
		std::cout << "-----------------------------------------" << std::endl;

		//histogram
		hydra::DenseHistogram<double, 3, hydra::device::sys_t> Hist_Data(
				{100, 100, 100},
				{min, min, min},
				{max, max, max });

		Hist_Data.Fill(range.begin(), range.end());


#ifdef _ROOT_AVAILABLE_

		for(size_t i=0;  i<100; i++){
			for(size_t j=0;  j<100; j++){
				for(size_t k=0;  k<100; k++){

					hist_data_d.SetBinContent(hist_data_d.GetBin(i+1, j+1, k+1),
							Hist_Data.GetBinContent({i,j,k} )  );
				}
			}
		}

		for(size_t i=1; i< 101; i++ ) {
			for(size_t j=1; j< 101; j++ ) {
				for(size_t k=1; k< 101; k++ ) {

					double x = hist_mc_d.GetXaxis()->GetBinCenter(i);
					double y = hist_mc_d.GetYaxis()->GetBinCenter(j);
					double z = hist_mc_d.GetZaxis()->GetBinCenter(k);

					auto value = hydra::make_tuple( x,y,z);
					hist_mc_d.SetBinContent(hist_mc_d.GetBin(i,j,k), fcn.GetPDF().GetFunctor()(value));

				}
			}
		}

		hist_mc_d.Scale(hist_data_d.Integral()/hist_mc_d.Integral() );


#endif //_ROOT_AVAILABLE_

	}

#ifdef _ROOT_AVAILABLE_
	TApplication *myapp=new TApplication("myapp",0,0);

	//draw histograms
	TCanvas canvas_d("canvas_d" ,"Distributions - Device", 1500, 500);
	canvas_d.Divide(3,1);
	canvas_d.cd(1);
	hist_data_d.Project3D("x")->Draw("hist");
	hist_mc_d.Project3D("x")->Draw("chistsame");
	hist_mc_d.Project3D("x")->SetLineColor(2);

	canvas_d.cd(2);
	hist_data_d.Project3D("y")->Draw("hist");
	hist_mc_d.Project3D("y")->Draw("chistsame");
	hist_mc_d.Project3D("y")->SetLineColor(2);

	canvas_d.cd(3);
	hist_data_d.Project3D("z")->Draw("hist");
	hist_mc_d.Project3D("z")->Draw("chistsame");
	hist_mc_d.Project3D("z")->SetLineColor(2);

	TCanvas canvas2_d("canvas_d" ,"Distributions - Device", 1000, 500);
	canvas2_d.Divide(2,1);
	canvas2_d.cd(1);
	hist_data_d.Draw("BOX1");
	canvas2_d.cd(2);
	hist_mc_d.Draw("iso");

	myapp->Run();

#endif //_ROOT_AVAILABLE_

	return 0;

}
#endif /* MULTIDIMENSIONAL_FIT_INL_ */
