package edu.wpi.first.wpilib.plugins.simulation.installer;

import java.io.InputStream;

import org.eclipse.jface.preference.IPreferenceStore;

import edu.wpi.first.wpilib.plugins.core.WPILibCore;
import edu.wpi.first.wpilib.plugins.core.installer.AbstractInstaller;
import edu.wpi.first.wpilib.plugins.simulation.WPILibSimulationPlugin;
import edu.wpi.first.wpilib.plugins.simulation.preferences.PreferenceConstants;


public class SimulationInstaller extends AbstractInstaller {

	public SimulationInstaller(String version) {
		super(version, WPILibSimulationPlugin.getDefault().getPreferenceStore().getString(PreferenceConstants.LIBRARY_INSTALLED), WPILibSimulationPlugin.getDefault().getSimulationDir());
	}
	
	
	@Override
	protected String getFeatureName() {
		return "simulation";
	}

	@Override
	protected void updateInstalledVersion(String version) {
		IPreferenceStore prefs = WPILibSimulationPlugin.getDefault().getPreferenceStore();
		prefs.setValue(PreferenceConstants.LIBRARY_INSTALLED, version);
		
	}

	@Override
	protected InputStream getInstallResourceStream() {
		return SimulationInstaller.class.getResourceAsStream("/resources/simulation.zip");
	}
}


