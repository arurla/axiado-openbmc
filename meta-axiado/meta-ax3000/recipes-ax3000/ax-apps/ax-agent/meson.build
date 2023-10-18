project('ax-agent', 'cpp',
    meson_version: '>= 0.61.3',
    default_options: [
        'buildtype=debugoptimized',
        'cpp_std=c++20',
        'warning_level=3',
        'werror=true',
        'generate_md=' + (meson.is_subproject() ? 'false' : 'true'),
    ],
    version: '1.0.0',
)

conf = configuration_data()

# DBus information
conf.set_quoted('AX_AGENT_INTERFACE', 'com.axiado.ax200.Demo')
conf.set_quoted('AX_AGENT_BUSNAME', 'com.axiado.ax200.Demo')
conf.set_quoted('AX_AGENT_PATH', '/com/axiado/ax200/Demo')

conf.set_quoted('AX_AGENT_SEND_DATA', 'SendDemoData')

conf.set_quoted('RW_ATTACK_PATH', '/com/axiado/ax200/Demo/RWAttack')
conf.set_quoted('HSM_KEYGEN_PATH', '/com/axiado/ax200/Demo/Gen_HSMKey')
conf.set_quoted('NW_ANOM_PATH', '/com/axiado/ax200/Demo/nwAnom')

configure_file(output: 'config.h', configuration: conf)

sdbusplus_dep = dependency('sdbusplus')
pdi_dep = dependency('phosphor-dbus-interfaces')
executable('axdemo',
    'ax-demo.cpp',
    dependencies:[
      pdi_dep,
      sdbusplus_dep,
      ],
    install: true,
    )

sdbusplus_dep = dependency('sdbusplus')
pdi_dep = dependency('phosphor-dbus-interfaces')
executable('ax-demo-test',
    'ax-demo-test.cpp',
    dependencies:[
      pdi_dep,
      sdbusplus_dep,
      ],
    install: true,
    )

sdbusplus_dep = dependency('sdbusplus')
pdi_dep = dependency('phosphor-dbus-interfaces')
executable('ax-demo-rw-test',
    'ax-demo-rw.cpp',
    dependencies:[
      pdi_dep,
      sdbusplus_dep,
      ],
    install: true,
    )

sdbusplus_dep = dependency('sdbusplus')
pdi_dep = dependency('phosphor-dbus-interfaces')
executable('bmc-ipc-agent',
    'ax-bmc-ipc-device.cpp',
    'ax-bmc-ipc-main.cpp',
    dependencies:[
      pdi_dep,
      sdbusplus_dep,
      ],
    install: true,
    )

sdbusplus_dep = dependency('sdbusplus')
pdi_dep = dependency('phosphor-dbus-interfaces')
executable('hsmkey-test',
    'HSMKey-demo-test.cpp',
    dependencies:[
      pdi_dep,
      sdbusplus_dep,
      ],
    install: true,
    )

executable('ax-nw-demo-test',
    'ax-nw-test.cpp',
    install: true,
    )