#include <iostream>
#include <cmath>
#include <fstream>

#include "TCanvas.h"
#include "TGraph2D.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TFile.h"
#include "TH2D.h"
#include "TBox.h"
#include "TPad.h"


void Weighted_efficiency() {
    const int n_pos = 13;
    const int n_energies = 9;

    double x_pos[n_pos]  = {0.0, 1.2, -1.2, 0.0, 0.0, 1.6, 1.6, -1.6, -1.6, 2.0, -2.0, 0.0, 0.0};
    double y_pos[n_pos]  = {0.0, 0.0, 0.0, 1.2, -1.2, 1.6, -1.6, -1.6, 1.6, 0.0, 0.0, 2.0, -2.0};
    double energies[n_energies] = {121.53, 244.6, 344.3, 778.9, 867.3, 964.0, 1085.9, 1112.0, 1408.0};

    // Efficiency Matrix
    double eff_p[n_pos][n_energies] = {
        {1.21, 3.54, 3.41, 2.32, 2.20, 2.16, 2.08, 2.00, 1.80},
        {1.92, 4.82, 4.58, 2.86, 2.80, 2.69, 2.55, 2.43, 2.11},
        {0.99, 2.93, 2.89, 2.01, 1.95, 1.87, 1.82, 1.74, 1.57},
        {1.45, 4.03, 3.80, 2.45, 2.45, 2.29, 2.21, 2.13, 1.87},
        {1.29, 3.57, 3.49, 2.31, 2.23, 2.16, 2.06, 1.97, 1.77},
        {3.51, 6.48, 5.81, 3.46, 3.35, 3.11, 2.94, 2.88, 2.47},
        {3.03, 5.92, 5.32, 3.15, 3.07, 2.91, 2.82, 2.63, 2.32},
        {1.11, 3.15, 2.98, 1.99, 1.95, 1.87, 1.82, 1.72, 1.54},
        {1.49, 3.48, 3.32, 2.16, 2.12, 1.97, 1.93, 1.83, 1.64},
        {5.65, 7.65, 6.53, 3.56, 3.55, 3.42, 3.20, 3.10, 2.62},
        {0.90, 2.72, 2.63, 1.86, 1.75, 1.69, 1.69, 1.61, 1.44},
        {2.97, 5.13, 4.58, 2.82, 2.66, 2.56, 2.44, 2.34, 2.04},
        {2.24, 4.51, 4.10, 2.55, 2.55, 2.33, 2.25, 2.16, 1.89}
    };

    double sigma = 1.08; // Standard deviation for Gaussian weighting
    double weight[n_pos];
    double weighted_eff[n_pos][n_energies];
    double total_weight = 0.0;

    // Open output file streams
    std::ofstream csv_out("gaussian_weighted_averaged_efficiencies.csv");
    // std::ofstream txt_out("gaussian_weighted_averaged_efficiencies.txt");

    if (!csv_out.is_open() /*|| !txt_out.is_open()*/) {
        std::cerr << "Error: Could not open output files!" << std::endl;
        return;
    }

    // Write headers
    csv_out << "Energy_keV,Weighted_Efficiency_Percent\n";
    // txt_out << "Energy (keV)\tWeighted Efficiency (%)\n";
    // txt_out << "-------------------------------------------\n";

    
    double final_eff = 0.0;
    for (int k = 0; k < n_pos; k++) {
        weight[k] = exp(-(x_pos[k]*x_pos[k] + y_pos[k]*y_pos[k])/ (2*sigma*sigma));
        // std::cout << "Position " << x_pos[k] << "," << y_pos[k] << " Weight: " << weight[k] << std::endl;
        total_weight += weight[k];
        // std::cout << "Total weight so far: " << total_weight << std::endl;
    }
    // Calculate Weight for each position based on its area

        
    // Calculate the weighted efficiency contribution for this position
    for (int i = 0; i < n_energies; i++) {
        final_eff = 0.0; // Reset for the next energy

        for (int j = 0; j < n_pos; j++) {
            // std::cout << "Previous efficiency for Energy " << energies[i] << " keV: " << eff_p[j][i] << std::endl;
            weighted_eff[j][i] = eff_p[j][i] * weight[j];
            // std::cout << "Current efficiency for Energy " << energies[i] << " keV, Weighted Efficiency: " << weighted_eff[j][i] << std::endl;

            final_eff += weighted_eff[j][i];
            // std::cout << "Final efficiency for Energy " << energies[i] << " keV so far: " << final_eff << std::endl;
            // std::cout << "======================================================" << std::endl;

        }
        // Calculate the weighted average efficiency for this energy
        double weighted_avg_eff = final_eff / total_weight;
        // std::cout << "Weighted average efficiency for Energy " << energies[i] << " keV: " << weighted_avg_eff << std::endl;
        

        // Save to CSV (comma-separated)
        csv_out << energies[i] << "," << weighted_avg_eff << "\n";


    }

    csv_out.close();
    // txt_out.close();

    std::cout << "\n[INFO] Files successfully created:" << std::endl;
    std::cout << "  -> gaussian_weighted_averaged_efficiencies.csv" << std::endl;
    std::cout << "  -> gaussian_weighted_averaged_efficiencies.txt" << std::endl;

    //======= Visualization of weighted efficiencies =======//
    TFile *outFile = TFile::Open("Heatmap_Weighted_efficiency.root", "RECREATE");

    double z_eff[n_pos];
    
    for(int e = 0; e < n_energies; e++){

        // Setup Canvas and Palette
        TCanvas *c1 = new TCanvas("c1", "Spatial Efficiency Heatmap", 800, 600);
        c1->SetRightMargin(0.16); // Leave room on the right for the color scale bar
        // gStyle->SetPalette(kBird); // kBird is a modern, colorblind-friendly ROOT heatmap palette

        // Create 2D Graph
        TGraph2D *g_heatmap = new TGraph2D(n_pos, x_pos, y_pos, z_eff);
        for (int p = 0; p < n_pos; p++) {
            // std::cout << "Weighted efficiency for Energy " << energies[e] << ":" << weighted_eff[p][e] << std::endl;
            z_eff[p] = weighted_eff[p][e];
            g_heatmap->SetPoint(p, x_pos[p], y_pos[p], z_eff[p]);
        }
        
        // g_heatmap->SetName(Form("Heatmap_%.2f_keV", energies[e]));
        g_heatmap->SetTitle(Form("Spatial Efficiency Profile at %.2f keV;X Position (cm);Y Position (cm);Efficiency (%%)", energies[e]));
        
        // Set Axis Title Offsets
        g_heatmap->GetXaxis()->SetTitleOffset(1.5);
        g_heatmap->GetYaxis()->SetTitleOffset(1.5);
        g_heatmap->GetZaxis()->SetTitleOffset(1.2);

        g_heatmap->SetNpx(60);
        g_heatmap->SetNpy(60);

        // Draw the heatmap
        // "COLZ" creates the interpolated colored heatmap with a Z-axis scale bar
        g_heatmap->Draw("COLZ");
        
        // "P0 SAME" overlays the actual 13 sampling coordinates as black dots
        // so you can see exactly where your simulation data points lie
        g_heatmap->SetMarkerStyle(20);
        g_heatmap->SetMarkerColor(kBlack);
        g_heatmap->SetMarkerSize(1.2);
        g_heatmap->Draw("P0 SAME");

        c1->Update();

        // Optional: Format the newly generated Z-axis so it looks clean
        g_heatmap->GetZaxis()->SetDecimals(kTRUE);
        g_heatmap->GetZaxis()->SetNoExponent(kTRUE);

        g_heatmap->SetMinimum(0.0);
        g_heatmap->SetMaximum(4.5);
        c1->Update();

        c1->Write(Form("Efficiency_Heatmap_%.2f_keV", energies[e]));

        g_heatmap->Write();
    }

    

    outFile->cd();
    outFile->Close();

}
    