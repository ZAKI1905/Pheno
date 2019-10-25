# This file was automatically created by FeynRules 2.3.32
# Mathematica version: 11.3.0 for Mac OS X x86 (64-bit) (March 7, 2018)
# Date: Sat 21 Apr 2018 20:48:39


from object_library import all_couplings, Coupling

from function_library import complexconjugate, re, im, csc, sec, acsc, asec, cot



GC_1 = Coupling(name = 'GC_1',
                value = '-(AH*complex(0,1))',
                order = {'HIW':1})

GC_2 = Coupling(name = 'GC_2',
                value = '-(ee*complex(0,1))/3.',
                order = {'QED':1})

GC_3 = Coupling(name = 'GC_3',
                value = '(2*ee*complex(0,1))/3.',
                order = {'QED':1})

GC_4 = Coupling(name = 'GC_4',
                value = '-(ee*complex(0,1))',
                order = {'QED':1})

GC_5 = Coupling(name = 'GC_5',
                value = 'ee*complex(0,1)',
                order = {'QED':1})

GC_6 = Coupling(name = 'GC_6',
                value = 'ee**2*complex(0,1)',
                order = {'QED':2})

GC_7 = Coupling(name = 'GC_7',
                value = '-G',
                order = {'QCD':1})

GC_8 = Coupling(name = 'GC_8',
                value = 'complex(0,1)*G',
                order = {'QCD':1})

GC_9 = Coupling(name = 'GC_9',
                value = 'complex(0,1)*G**2',
                order = {'QCD':2})

GC_10 = Coupling(name = 'GC_10',
                 value = '-(complex(0,1)*GH)',
                 order = {'HIG':1})

GC_11 = Coupling(name = 'GC_11',
                 value = '-(G*GH)',
                 order = {'HIG':1,'QCD':1})

GC_12 = Coupling(name = 'GC_12',
                 value = 'complex(0,1)*G**2*GH',
                 order = {'HIG':1,'QCD':2})

GC_13 = Coupling(name = 'GC_13',
                 value = '(complex(0,1)*Gphi)/8.',
                 order = {'HIG':1})

GC_14 = Coupling(name = 'GC_14',
                 value = '(G*Gphi)/4.',
                 order = {'HIG':1,'QCD':1})

GC_15 = Coupling(name = 'GC_15',
                 value = '-(complex(0,1)*kq)',
                 order = {'QED':3})

GC_16 = Coupling(name = 'GC_16',
                 value = '-6*complex(0,1)*lam',
                 order = {'QED':2})

GC_17 = Coupling(name = 'GC_17',
                 value = '-2*complex(0,1)*lamf',
                 order = {'QED':1})

GC_18 = Coupling(name = 'GC_18',
                 value = '(ee**2*complex(0,1))/(2.*sw**2)',
                 order = {'QED':2})

GC_19 = Coupling(name = 'GC_19',
                 value = '-((ee**2*complex(0,1))/sw**2)',
                 order = {'QED':2})

GC_20 = Coupling(name = 'GC_20',
                 value = '(cw**2*ee**2*complex(0,1))/sw**2',
                 order = {'QED':2})

GC_21 = Coupling(name = 'GC_21',
                 value = '(ee*complex(0,1))/(sw*cmath.sqrt(2))',
                 order = {'QED':1})

GC_22 = Coupling(name = 'GC_22',
                 value = '(CKM1x1*ee*complex(0,1))/(sw*cmath.sqrt(2))',
                 order = {'QED':1})

GC_23 = Coupling(name = 'GC_23',
                 value = '(CKM1x2*ee*complex(0,1))/(sw*cmath.sqrt(2))',
                 order = {'QED':1})

GC_24 = Coupling(name = 'GC_24',
                 value = '(CKM1x3*ee*complex(0,1))/(sw*cmath.sqrt(2))',
                 order = {'QED':1})

GC_25 = Coupling(name = 'GC_25',
                 value = '(CKM2x1*ee*complex(0,1))/(sw*cmath.sqrt(2))',
                 order = {'QED':1})

GC_26 = Coupling(name = 'GC_26',
                 value = '(CKM2x2*ee*complex(0,1))/(sw*cmath.sqrt(2))',
                 order = {'QED':1})

GC_27 = Coupling(name = 'GC_27',
                 value = '(CKM2x3*ee*complex(0,1))/(sw*cmath.sqrt(2))',
                 order = {'QED':1})

GC_28 = Coupling(name = 'GC_28',
                 value = '(CKM3x1*ee*complex(0,1))/(sw*cmath.sqrt(2))',
                 order = {'QED':1})

GC_29 = Coupling(name = 'GC_29',
                 value = '(CKM3x2*ee*complex(0,1))/(sw*cmath.sqrt(2))',
                 order = {'QED':1})

GC_30 = Coupling(name = 'GC_30',
                 value = '(CKM3x3*ee*complex(0,1))/(sw*cmath.sqrt(2))',
                 order = {'QED':1})

GC_31 = Coupling(name = 'GC_31',
                 value = '(cw*ee*complex(0,1))/sw',
                 order = {'QED':1})

GC_32 = Coupling(name = 'GC_32',
                 value = '(-2*cw*ee**2*complex(0,1))/sw',
                 order = {'QED':2})

GC_33 = Coupling(name = 'GC_33',
                 value = '(ee*complex(0,1)*sw)/(3.*cw)',
                 order = {'QED':1})

GC_34 = Coupling(name = 'GC_34',
                 value = '(-2*ee*complex(0,1)*sw)/(3.*cw)',
                 order = {'QED':1})

GC_35 = Coupling(name = 'GC_35',
                 value = '(ee*complex(0,1)*sw)/cw',
                 order = {'QED':1})

GC_36 = Coupling(name = 'GC_36',
                 value = '-(cw*ee*complex(0,1))/(2.*sw) - (ee*complex(0,1)*sw)/(6.*cw)',
                 order = {'QED':1})

GC_37 = Coupling(name = 'GC_37',
                 value = '(cw*ee*complex(0,1))/(2.*sw) - (ee*complex(0,1)*sw)/(6.*cw)',
                 order = {'QED':1})

GC_38 = Coupling(name = 'GC_38',
                 value = '-(cw*ee*complex(0,1))/(2.*sw) + (ee*complex(0,1)*sw)/(2.*cw)',
                 order = {'QED':1})

GC_39 = Coupling(name = 'GC_39',
                 value = '(cw*ee*complex(0,1))/(2.*sw) + (ee*complex(0,1)*sw)/(2.*cw)',
                 order = {'QED':1})

GC_40 = Coupling(name = 'GC_40',
                 value = 'ee**2*complex(0,1) + (cw**2*ee**2*complex(0,1))/(2.*sw**2) + (ee**2*complex(0,1)*sw**2)/(2.*cw**2)',
                 order = {'QED':2})

GC_41 = Coupling(name = 'GC_41',
                 value = '-(complex(0,1)*kq*vev)',
                 order = {'QED':2})

GC_42 = Coupling(name = 'GC_42',
                 value = '-6*complex(0,1)*lam*vev',
                 order = {'QED':1})

GC_43 = Coupling(name = 'GC_43',
                 value = '(ee**2*complex(0,1)*vev)/(2.*sw**2)',
                 order = {'QED':1})

GC_44 = Coupling(name = 'GC_44',
                 value = 'ee**2*complex(0,1)*vev + (cw**2*ee**2*complex(0,1)*vev)/(2.*sw**2) + (ee**2*complex(0,1)*sw**2*vev)/(2.*cw**2)',
                 order = {'QED':1})

GC_45 = Coupling(name = 'GC_45',
                 value = '-((complex(0,1)*yb)/cmath.sqrt(2))',
                 order = {'QED':1})

GC_46 = Coupling(name = 'GC_46',
                 value = '-((complex(0,1)*yc)/cmath.sqrt(2))',
                 order = {'QED':1})

GC_47 = Coupling(name = 'GC_47',
                 value = '-((complex(0,1)*ydo)/cmath.sqrt(2))',
                 order = {'QED':1})

GC_48 = Coupling(name = 'GC_48',
                 value = '-((complex(0,1)*ye)/cmath.sqrt(2))',
                 order = {'QED':1})

GC_49 = Coupling(name = 'GC_49',
                 value = '-((complex(0,1)*yf1x1)/cmath.sqrt(2))',
                 order = {'QED':2})

GC_50 = Coupling(name = 'GC_50',
                 value = '-((complex(0,1)*vev*yf1x1)/cmath.sqrt(2))',
                 order = {'QED':1})

GC_51 = Coupling(name = 'GC_51',
                 value = '-((complex(0,1)*yf1x2)/cmath.sqrt(2))',
                 order = {'QED':2})

GC_52 = Coupling(name = 'GC_52',
                 value = '-((complex(0,1)*vev*yf1x2)/cmath.sqrt(2))',
                 order = {'QED':1})

GC_53 = Coupling(name = 'GC_53',
                 value = '-((complex(0,1)*yf1x3)/cmath.sqrt(2))',
                 order = {'QED':2})

GC_54 = Coupling(name = 'GC_54',
                 value = '-((complex(0,1)*vev*yf1x3)/cmath.sqrt(2))',
                 order = {'QED':1})

GC_55 = Coupling(name = 'GC_55',
                 value = '-((complex(0,1)*yf2x1)/cmath.sqrt(2))',
                 order = {'QED':2})

GC_56 = Coupling(name = 'GC_56',
                 value = '-((complex(0,1)*vev*yf2x1)/cmath.sqrt(2))',
                 order = {'QED':1})

GC_57 = Coupling(name = 'GC_57',
                 value = '-((complex(0,1)*yf2x2)/cmath.sqrt(2))',
                 order = {'QED':2})

GC_58 = Coupling(name = 'GC_58',
                 value = '-((complex(0,1)*vev*yf2x2)/cmath.sqrt(2))',
                 order = {'QED':1})

GC_59 = Coupling(name = 'GC_59',
                 value = '-((complex(0,1)*yf2x3)/cmath.sqrt(2))',
                 order = {'QED':2})

GC_60 = Coupling(name = 'GC_60',
                 value = '-((complex(0,1)*vev*yf2x3)/cmath.sqrt(2))',
                 order = {'QED':1})

GC_61 = Coupling(name = 'GC_61',
                 value = '-((complex(0,1)*yf3x1)/cmath.sqrt(2))',
                 order = {'QED':2})

GC_62 = Coupling(name = 'GC_62',
                 value = '-((complex(0,1)*vev*yf3x1)/cmath.sqrt(2))',
                 order = {'QED':1})

GC_63 = Coupling(name = 'GC_63',
                 value = '-((complex(0,1)*yf3x2)/cmath.sqrt(2))',
                 order = {'QED':2})

GC_64 = Coupling(name = 'GC_64',
                 value = '-((complex(0,1)*vev*yf3x2)/cmath.sqrt(2))',
                 order = {'QED':1})

GC_65 = Coupling(name = 'GC_65',
                 value = '-((complex(0,1)*yf3x3)/cmath.sqrt(2))',
                 order = {'QED':2})

GC_66 = Coupling(name = 'GC_66',
                 value = '-((complex(0,1)*vev*yf3x3)/cmath.sqrt(2))',
                 order = {'QED':1})

GC_67 = Coupling(name = 'GC_67',
                 value = '-((complex(0,1)*ym)/cmath.sqrt(2))',
                 order = {'QED':1})

GC_68 = Coupling(name = 'GC_68',
                 value = '-((complex(0,1)*ys)/cmath.sqrt(2))',
                 order = {'QED':1})

GC_69 = Coupling(name = 'GC_69',
                 value = '-((complex(0,1)*yt)/cmath.sqrt(2))',
                 order = {'QED':1})

GC_70 = Coupling(name = 'GC_70',
                 value = '-((complex(0,1)*ytau)/cmath.sqrt(2))',
                 order = {'QED':1})

GC_71 = Coupling(name = 'GC_71',
                 value = '-((complex(0,1)*yup)/cmath.sqrt(2))',
                 order = {'QED':1})

GC_72 = Coupling(name = 'GC_72',
                 value = '(ee*complex(0,1)*complexconjugate(CKM1x1))/(sw*cmath.sqrt(2))',
                 order = {'QED':1})

GC_73 = Coupling(name = 'GC_73',
                 value = '(ee*complex(0,1)*complexconjugate(CKM1x2))/(sw*cmath.sqrt(2))',
                 order = {'QED':1})

GC_74 = Coupling(name = 'GC_74',
                 value = '(ee*complex(0,1)*complexconjugate(CKM1x3))/(sw*cmath.sqrt(2))',
                 order = {'QED':1})

GC_75 = Coupling(name = 'GC_75',
                 value = '(ee*complex(0,1)*complexconjugate(CKM2x1))/(sw*cmath.sqrt(2))',
                 order = {'QED':1})

GC_76 = Coupling(name = 'GC_76',
                 value = '(ee*complex(0,1)*complexconjugate(CKM2x2))/(sw*cmath.sqrt(2))',
                 order = {'QED':1})

GC_77 = Coupling(name = 'GC_77',
                 value = '(ee*complex(0,1)*complexconjugate(CKM2x3))/(sw*cmath.sqrt(2))',
                 order = {'QED':1})

GC_78 = Coupling(name = 'GC_78',
                 value = '(ee*complex(0,1)*complexconjugate(CKM3x1))/(sw*cmath.sqrt(2))',
                 order = {'QED':1})

GC_79 = Coupling(name = 'GC_79',
                 value = '(ee*complex(0,1)*complexconjugate(CKM3x2))/(sw*cmath.sqrt(2))',
                 order = {'QED':1})

GC_80 = Coupling(name = 'GC_80',
                 value = '(ee*complex(0,1)*complexconjugate(CKM3x3))/(sw*cmath.sqrt(2))',
                 order = {'QED':1})

GC_81 = Coupling(name = 'GC_81',
                 value = '-((complex(0,1)*complexconjugate(yf1x1))/cmath.sqrt(2))',
                 order = {'QED':2})

GC_82 = Coupling(name = 'GC_82',
                 value = '-((complex(0,1)*vev*complexconjugate(yf1x1))/cmath.sqrt(2))',
                 order = {'QED':1})

GC_83 = Coupling(name = 'GC_83',
                 value = '-((complex(0,1)*complexconjugate(yf1x2))/cmath.sqrt(2))',
                 order = {'QED':2})

GC_84 = Coupling(name = 'GC_84',
                 value = '-((complex(0,1)*vev*complexconjugate(yf1x2))/cmath.sqrt(2))',
                 order = {'QED':1})

GC_85 = Coupling(name = 'GC_85',
                 value = '-((complex(0,1)*complexconjugate(yf1x3))/cmath.sqrt(2))',
                 order = {'QED':2})

GC_86 = Coupling(name = 'GC_86',
                 value = '-((complex(0,1)*vev*complexconjugate(yf1x3))/cmath.sqrt(2))',
                 order = {'QED':1})

GC_87 = Coupling(name = 'GC_87',
                 value = '-((complex(0,1)*complexconjugate(yf2x1))/cmath.sqrt(2))',
                 order = {'QED':2})

GC_88 = Coupling(name = 'GC_88',
                 value = '-((complex(0,1)*vev*complexconjugate(yf2x1))/cmath.sqrt(2))',
                 order = {'QED':1})

GC_89 = Coupling(name = 'GC_89',
                 value = '-((complex(0,1)*complexconjugate(yf2x2))/cmath.sqrt(2))',
                 order = {'QED':2})

GC_90 = Coupling(name = 'GC_90',
                 value = '-((complex(0,1)*vev*complexconjugate(yf2x2))/cmath.sqrt(2))',
                 order = {'QED':1})

GC_91 = Coupling(name = 'GC_91',
                 value = '-((complex(0,1)*complexconjugate(yf2x3))/cmath.sqrt(2))',
                 order = {'QED':2})

GC_92 = Coupling(name = 'GC_92',
                 value = '-((complex(0,1)*vev*complexconjugate(yf2x3))/cmath.sqrt(2))',
                 order = {'QED':1})

GC_93 = Coupling(name = 'GC_93',
                 value = '-((complex(0,1)*complexconjugate(yf3x1))/cmath.sqrt(2))',
                 order = {'QED':2})

GC_94 = Coupling(name = 'GC_94',
                 value = '-((complex(0,1)*vev*complexconjugate(yf3x1))/cmath.sqrt(2))',
                 order = {'QED':1})

GC_95 = Coupling(name = 'GC_95',
                 value = '-((complex(0,1)*complexconjugate(yf3x2))/cmath.sqrt(2))',
                 order = {'QED':2})

GC_96 = Coupling(name = 'GC_96',
                 value = '-((complex(0,1)*vev*complexconjugate(yf3x2))/cmath.sqrt(2))',
                 order = {'QED':1})

GC_97 = Coupling(name = 'GC_97',
                 value = '-((complex(0,1)*complexconjugate(yf3x3))/cmath.sqrt(2))',
                 order = {'QED':2})

GC_98 = Coupling(name = 'GC_98',
                 value = '-((complex(0,1)*vev*complexconjugate(yf3x3))/cmath.sqrt(2))',
                 order = {'QED':1})

