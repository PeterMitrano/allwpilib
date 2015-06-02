package edu.wpi.first.wpilib.plugins.simulation.installer;

import java.io.InputStream;

import org.eclipse.jface.preference.IPreferenceStore;

import edu.wpi.first.wpilib.plugins.core.installer.AbstractInstaller;
import edu.wpi.first.wpilib.plugins.simulation.WPILibJavaPlugin;
import edu.wpi.first.wpilib.plugins.simulation.preferences.PreferenceConstants;

/**
 * Installs the given version of gazebo plugins and models into the correct location. Where the
 * install directory is usually ~/wpilib/simulation/version.
 * 
 * @author Peter Mitrano <peter@osrfoundation.com>
 */
public class SimulationInstaller extends AbstractInstaller {

  public SimulationInstaller(String version) {
    super(version,
        WPILibJavaPlugin.getDefault().getPreferenceStore().getString(PreferenceConstants.LIBRARY_INSTALLED),
        WPILibJavaPlugin.getDefault().getJavaPath());
  }

  @Override
  protected String getFeatureName() {
    return "simulation";
  }

  @Override
  protected void updateInstalledVersion(String version) {
    IPreferenceStore prefs = WPILibJavaPlugin.getDefault().getPreferenceStore();
      prefs.setValue(PreferenceConstants.LIBRARY_INSTALLED, version);
  }

  @Override
  protected InputStream getInstallResourceStream() {
    return SimulationInstaller.class.getResourceAsStream("/resources/simulation.zip");
  }
}
