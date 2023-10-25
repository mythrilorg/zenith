package emu.zenith.settings

import android.content.Intent
import android.os.Bundle
import android.provider.Settings
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.WindowCompat
import androidx.recyclerview.widget.LinearLayoutManager
import com.google.android.material.radiobutton.MaterialRadioButton
import emu.zenith.R
import emu.zenith.adapters.SelectableViewAdapter
import emu.zenith.data.Permissions
import emu.zenith.databinding.KernelActivityBinding
import emu.zenith.helpers.KernelsHelper
import emu.zenith.helpers.PermissionHelper
import emu.zenith.helpers.views.KernelViewItem

class KernelActivity : AppCompatActivity() {
    private val binding by lazy {
        KernelActivityBinding.inflate(layoutInflater)
    }

    private val kernels by lazy { KernelsHelper(this) }
    private val adapter = SelectableViewAdapter(kernels.getRunningKernel(0))

    private lateinit var checkStorage: PermissionHelper

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(binding.root)
        WindowCompat.setDecorFitsSystemWindows(window, false)
        binding.appToolBar.apply {
            title = resources.getString(R.string.toolbar_kernel)
        }
        setSupportActionBar(binding.appToolBar)
        supportActionBar?.setDisplayHomeAsUpEnabled(true)

        // It needs to be created here because the Context may not be valid
        checkStorage = PermissionHelper(this, Permissions.storageAccess) {
            val launcher = Intent(Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION)
            startActivity(launcher)
        }

        val manager = LinearLayoutManager(this)
        binding.kernelRecycler.adapter = adapter
        binding.kernelRecycler.layoutManager = manager

        kernels.getKernels().forEachIndexed { index, kernel ->
            adapter.insertItem(index, KernelViewItem(this@KernelActivity, kernel).apply {
                onClick = {
                    kernels.activateKernel(index)
                    adapter.selectItem(index)
                    if (it is MaterialRadioButton && kernel.selected)
                        it.isChecked = true
                }
                onDelete = { _, _ ->
                    kernels.unloadKernel(index)
                }

                if (index == adapter.selectedPos) {
                    kernels.activateKernel(index)
                    adapter.selectItem(index)
                }
            })
        }
    }

    override fun onResume() {
        super.onResume()
        checkStorage.checkForPermission()
    }
}